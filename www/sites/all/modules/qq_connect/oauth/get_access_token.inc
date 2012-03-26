<?php
/**
 * PHP SDK for QQ登录 OpenAPI
 *
 * @version 1.3
 * @author connect@qq.com
 * @copyright © 2011, Tencent Corporation. All rights reserved.
 */

/**
 * @brief 获取access_token。请求需经过URL编码，编码时请遵循 RFC 1738
 *
 * @param $appid
 * @param $appkey
 * @param $request_token
 * @param $request_token_secret
 * @param $vericode
 *
 * @return 返回字符串格式为：oauth_token=xxx&oauth_token_secret=xxx&openid=xxx&oauth_signature=xxx&oauth_vericode=xxx&timestamp=xxx
 */

function get_access_token($appid, $appkey, $request_token, $request_token_secret, $vericode)
{
    //请求具有Qzone访问权限的access_token的接口地址, 不要更改!!
    $url    = "http://openapi.qzone.qq.com/oauth/qzoneoauth_access_token?";
   
    //生成oauth_signature签名值。签名值生成方法详见（http://wiki.opensns.qq.com/wiki/【QQ登录】签名参数oauth_signature的说明）
    //（1） 构造生成签名值的源串（HTTP请求方式 & urlencode(uri) & urlencode(a=x&b=y&...)）
	$sigstr = "GET"."&".rawurlencode("http://openapi.qzone.qq.com/oauth/qzoneoauth_access_token")."&";

    //必要参数，不要随便更改!!
    $params = array();
    $params["oauth_version"]          = "1.0";
    $params["oauth_signature_method"] = "HMAC-SHA1";
    $params["oauth_timestamp"]        = time();
    $params["oauth_nonce"]            = mt_rand();
    $params["oauth_consumer_key"]     = $appid;
    $params["oauth_token"]            = $request_token;
    $params["oauth_vericode"]         = $vericode;

    //对参数按照字母升序做序列化
    $normalized_str = get_normalized_string($params);
    $sigstr        .= rawurlencode($normalized_str);

  	//（2）构造密钥
    $key = $appkey."&".$request_token_secret;

	  //（3）生成oauth_signature签名值。这里需要确保PHP版本支持hash_hmac函数
    $signature = get_signature($sigstr, $key);
    
	
	//构造请求url
    $url      .= $normalized_str."&"."oauth_signature=".rawurlencode($signature);

    return file_get_contents($url);
}

/**
 * page callback.
 * */
function _qq_connect_callback() {
  //用户使用QQ登录，并授权成功后，会返回用户的openid。此时需要检查返回的openid是否是合法id
  //我们不建议开发者使用该openid，而是使用获取access token之后返回的openid。
  if (!is_valid_openid($_REQUEST["openid"], $_REQUEST["timestamp"], $_REQUEST["oauth_signature"])) {
    //demo对错误简单处理
    echo "###invalid openid\n";
    echo "sig:".$_REQUEST["oauth_signature"]."\n";
    exit;
  }
  
  //用授权的request token换取access token
  $access_str = get_access_token($_SESSION["appid"], $_SESSION["appkey"], $_REQUEST["oauth_token"], $_SESSION["secret"], $_REQUEST["oauth_vericode"]);
  $result = array();
  parse_str($access_str, $result);
  
  //错误处理
  if (isset($result["error_code"])) {
    echo "get access token error<br>";
    echo "error msg: $request_token<br>";
    echo "点击<a href=\"http://wiki.opensns.qq.com/wiki/%E3%80%90QQ%E7%99%BB%E5%BD%95%E3%80%91%E5%85%AC%E5%85%B1%E8%BF%94%E5%9B%9E%E7%A0%81%E8%AF%B4%E6%98%8E\" target=_blank>查看错误码</a>";
    exit;
  }
  
  //将access token，openid保存起来！！！
  //在demo演示中，直接保存在全局变量中.
  //为避免网站存在多个子域名或同一个主域名不同服务器造成的session无法共享问题 
  //请开发者按照本SDK中comm/session.php中的注释对session.php进行必要的修改，以解决上述2个问题，
  $_SESSION["token"]   = $result["oauth_token"];
  $_SESSION["secret"]  = $result["oauth_token_secret"]; 
  $_SESSION["openid"]  = $result["openid"];
  
  $user_info = get_user_info($_SESSION["appid"], $_SESSION["appkey"], $_SESSION["token"], $_SESSION["secret"], $_SESSION["openid"]);
  $qq_name = $user_info["nickname"] ? $user_info["nickname"] : 'QQ nickname';
  $qq_name = check_plain($qq_name); //sanitize
  
  //检查openid是否绑定，已绑定则返回authname，未绑定则生成新绑定
  $authname = _qq_connect_bind_name($_SESSION["openid"], $qq_name);

  //login or register 
  user_external_login_register($authname, 'qq_connect');
  user_external_load($authname);
  
  //登录成功，同步name， picture -- 放到后期手动，以免登录过长，并造成页面老死
  
  $output = '<p>您已成功使用QQ登录本站，祝您使用愉快！</p>';
  
  header("Location:/");
  return $output;
  
}
