<?php

/**
 * Helper function.
 * 绑定openid与用户名.
 * 
 * @param $openid
 * @param $authname
 * 
 * @return authname
 */
function _qq_connect_bind_name($openid='', $authname='') {
  //check openid
  $exist_authname = _qq_connect_check_openid($openid);
  if ($exist_authname) {return $exist_authname;}
  
  //check authname, for new bind
  $available_authname = _qq_connect_check_authname($authname);
  if ($available_authname) {
    $new_bind = db_insert('qq_connect')
      ->fields(array(
        'openid' => $openid,
        'authname' => $available_authname,
      ))
      ->execute();
      
    return $available_authname; //@todo may need check insert status
  }
  
}

/**
 * Helper function.
 * 检查openid是否绑定.
 * 
 * @param $openid
 * 
 * @return authname or false
 */
function _qq_connect_check_openid($openid = '') {

  $result = db_query("SELECT openid, authname FROM {qq_connect} WHERE openid = :openid", array(':openid' => $openid));
  
  $number_of_rows = $result->rowCount();
  $record = $result->fetchObject();
  
  if ($number_of_rows == 1) {
    return $record->authname;
  }
  
  return false;
}

/**
 * Helper function.
 * 检查authname是否绑定.
 * 
 * @param $authname
 * 
 * @return available authname
 */
function _qq_connect_check_authname($authname = '') {
  
  $result = db_query("SELECT authname FROM {qq_connect} WHERE authname = :authname", array(':authname' => $authname));
  
  $number_of_rows = $result->rowCount();
  
  if ($number_of_rows > 0) {
    $authname = $authname . mt_rand(1, 99);
    
    $available_authname = false;
    while (!$available_authname) {
      $available_authname = _qq_connect_check_authname($authname);
    }
    
    return $available_authname;
  }
  
  return $authname;
}

/**
 * Helper function.
 * 获取QQ用户的头像，并保存.
 * 
 * @param 
 * 
 * @return 
 */
function _qq_connect_save_avatar($form, $form_state) {
  $directory = 'public://qq_connect';
  $qq_connect_dir = file_prepare_directory($directory, $options = FILE_CREATE_DIRECTORY); //not exist
  if(!$qq_connect_dir) {
    $qq_connect_dir = drupal_mkdir('public://qq_connect', $mode = NULL, $recursive = FALSE, $context = NULL); //create new
    if(!$qq_connect_dir) {
      drupal_set_message(t('Can not create qq_connect directory.'), $type = 'error', $repeat = TRUE);
      return false;
    }
    $qq_connect_dir = file_prepare_directory($directory, $options = FILE_MODIFY_PERMISSIONS); //make writeable
  }
  
  $user_info = get_user_info($_SESSION["appid"], $_SESSION["appkey"], $_SESSION["token"], $_SESSION["secret"], $_SESSION["openid"]);
  $avatar_url = $user_info["figureurl_2"] ? $user_info["figureurl_2"] : '';
  
  function get_data($url) {
    $ch = curl_init();
    $timeout = 5;
    curl_setopt($ch,CURLOPT_URL,$url);
    curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
    curl_setopt($ch,CURLOPT_CONNECTTIMEOUT,$timeout);
    $data = curl_exec($ch);
    curl_close($ch);
    return $data;
  }
  $file_str = get_data($avatar_url);
   
  global $user;
  $file = file_save_data($file_str, 'public://qq_connect/qq_avatar_' . $user->uid, FILE_EXISTS_REPLACE);
  if (!$file->fid) {return false;}
 
  $old_avatar = file_load($user->picture);
  
  //update new avatar
  $update_picture = db_update('users')
    ->fields(array(
        'picture' => $file->fid,
      ))
    ->condition('uid', $user->uid)
    ->execute();
    
  $output = '';
  $output .= '<div id="qq-avatar">';  
  $output .= '  <div class="user-picture">';
  $output .= '  <img title="'. $user->name .'的头像" alt="'. $user->name .'的头像" src="'. $avatar_url .'" typeof="foaf:Image">'; 
  $output .= '  </div>';
  $output .= '</div>';
  $output .= '';

  $account = user_load($user->uid, true);
  // Picture/avatar.
  $form['picture'] = array(
    '#type' => 'fieldset',
    '#title' => t('Picture'),
    '#weight' => 1,
    '#access' => true,
  );
  $form['picture']['picture'] = array(
    '#type' => 'value',
    '#value' => isset($account->picture) ? $account->picture : NULL,
  );
  $form['picture']['picture_current'] = array(
    '#markup' => theme('user_picture', array('account' => $account)),
  );
  $form['picture']['picture_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete picture'),
    '#access' => !empty($account->picture->fid),
    '#description' => t('Check this box to delete your current picture.'),
  );
  $form['picture']['picture_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload picture'),
    '#size' => 48,
    '#description' => t('Your virtual face or picture. Pictures larger than @dimensions pixels will be scaled down.', array('@dimensions' => variable_get('user_picture_dimensions', '85x85'))) . ' ' . filter_xss_admin(variable_get('user_picture_guidelines', '')),
  );
  $form['picture']['qq_avatar'] = array(
      '#type' => 'button',
      '#value' => t('从QQ空间下载'),
      '#name' => 'qq_dowload',
      '#ajax' => array(
        'callback' => '_qq_connect_save_avatar',
        'wrapper' => 'edit-picture',
        'trigger_as' => array('name' => 'qq_dowload'),
        'effect' => 'fade',
      ),
    );
  return $form['picture'];
}







