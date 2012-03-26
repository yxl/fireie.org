<?php
/**
 * PHP SDK for QQ登录 OpenAPI
 *
 * @version 1.3
 * @author connect@qq.com
 * @copyright © 2011, Tencent Corporation. All rights reserved.
 */

/**
 * @brief 本文件作为demo的配置文件。
 */

/**
 * 正式运营环境请关闭错误信息
 * ini_set("error_reporting", E_ALL);
 * ini_set("display_errors", TRUE);
 * QQDEBUG = true  开启错误提示
 * QQDEBUG = false 禁止错误提示
 * 默认禁止错误信息
 */
//define("QQDEBUG", false);
//if (defined("QQDEBUG") && QQDEBUG)
//{
//    @ini_set("error_reporting", E_ALL);
//    @ini_set("display_errors", TRUE);
//}

/**
 * session
 */
//include_once("session.php");
//currently, drupal handles the session crontrol


/**
 * 在你运行本demo之前请到 http://connect.opensns.qq.com/申请appid, appkey, 并注册callback地址
 */
//申请到的appid
//$_SESSION["appid"]    = yourappid; 

//申请到的appkey
//$_SESSION["appkey"]   = "yourappkey"; 

//QQ登录成功后跳转的地址,请确保地址真实可用，否则会导致登录失败。
//$_SESSION["callback"] = "http://your domain/user/login/qq/callback"; 

/**
 * Form builder. Configure QQ connect.
 * 
 * @ingroup forms
 * @see system_settings_form().
 */
function qq_connect_admin_settings() {
 $form['qq_connect_meta'] = array(
      '#type' => 'textfield',
      '#title' => t('meta verification content'),
      '#description' => t('Please input verification meta content of your application '),
      '#default_value' => variable_get('qq_connect_meta', '25271227125156727'),
  );
  $form['qq_connect_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('Appid for this site'),
    '#default_value' => variable_get('qq_connect_appid', '222222'),
    '#description' => t('Visit http://open.qq.com to get the appid. Test appid: 222222.'),
  );
  $form['qq_connect_appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Appkey for this site'),
    '#default_value' => variable_get('qq_connect_appkey', '005831692a444765a0db25a4a5ac052c'),
    '#description' => t('Visit http://open.qq.com to get the appkey. Test appkey: 005831692a444765a0db25a4a5ac052c.'),
  );
  $form['qq_connect_callback'] = array(
    '#type' => 'textfield',
    '#title' => t('Page callback after login successed.'),
    '#default_value' => variable_get('qq_connect_callback', 'http://fireie.org/zh-CN/user/login/qq/callback'),
    '#description' => t('Visit http://open.qq.com to get the appkey.'),
  );
  $form['qq_connect_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Turn on QQ connect debug information.'),
    '#default_value' => variable_get('qq_connect_debug', 1),
    '#description' => t('Make sure ncheck this on the production site. Clear cache when you change the above settings.'),
  );
  
  return system_settings_form($form);
}









