<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# githubog: http://ogp.me/ns/fb/githubog#">
    <meta http-equiv="Content-Type" charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>fireie.org</title>
  </head>
  <body>
<?php
   // execute `git pull` and save the result to file
   system("git pull > result.txt");
   // show the result
   $filename = "result.txt";
   $fp = fopen($filename, "r");
   $size = filesize($filename);
   $result = fread($fp, $size);
   fclose($fp);
   echo "<pre>" . $result . "</pre>";   
?>
  </body>
 </html>