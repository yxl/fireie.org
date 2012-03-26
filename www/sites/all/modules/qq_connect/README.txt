-- SUMMARY --

QQ connect provides external login with QQ account.

More detail information please visit http://open.qq.com. 

模块保留了QQ SDK相关版本的源文件，作为开发和维护的参考。
模块自身文件保持了与SDK相同的目录结构，以方便后期SDK升级后的维护工作。

Currently, Drupal 7 only.

-- FEATURES --
* Login with QQ account.

-- TODO --
* Add QQ login for login block -- done.
* Add a menu link for QQ login
* Add support for modifying user profile
* Add support for QQ 日志
* Add support for QQ 相册
* Add support for QQ 说说
* Add support for QQ 空间同步
* Add support for QQ 用户资料同步

-- REQUIREMENTS --
* None.

-- INSTALLATION --

* Install as usual.
* Apply appid & appkey from http://open.qq.com.
  申请说明： http://connect.opensns.qq.com/apply
  要求和注意：
    必须要备案号
    域名验证的时候把下载下来的txt放到drupl根目录下
    回调地址域名就是网站域名
  
* Set your appid & appkey on admin/config/people/qq-connect

-- ISSUE --
* 任何问题请发布在sandbox issue列表里或本模块的project申请页中，以防止相同问题重复提问。

-- CONTACT --

Current maintainers:
* rogical

This project has been sponsored by:
* rogical
