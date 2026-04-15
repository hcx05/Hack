
---
Sourcecode :

```php
<?
$key = "";

//input stored in $_REQUEST, maybe GET(URL), POST(Form) or Cookies
if(array_key_exists("needle", $_REQUEST)) {
    $key = $_REQUEST["needle"];
}

// passthru : execute command on Linux shell, so take advantage of it and use command injection
if($key != "") {
    passthru("grep -i $key dictionary.txt");
}
?>

```