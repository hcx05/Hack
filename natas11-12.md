
---
```php
$defaultdata = array( "showpassword"=>"no", "bgcolor"=>"#ffffff");
```

```php
function xor_encrypt($in) {
    $key = '<censored>';
    $text = $in;
    $outText = '';

    // Iterate through each character
    for($i=0;$i<strlen($text);$i++) {
    $outText .= $text[$i] ^ $key[$i % strlen($key)];
    }

    return $outText;
}
// Useing key and input do XOR encryption, key is usually shorter than plain text.
// $key[$i % strlen($key)]; to reuse key.
// A XOR B = A', A' XOR B = A. XOR is a symmetric operation so it's unsafe
// .= : += in php.
```

```php
function loadData($def) {
    global $_COOKIE;
    $mydata = $def;
    if(array_key_exists("data", $_COOKIE)) {
    $tempdata = json_decode(xor_encrypt(base64_decode($_COOKIE["data"])), true);
    if(is_array($tempdata) && array_key_exists("showpassword", $tempdata) && array_key_exists("bgcolor", $tempdata)) {
        if (preg_match('/^#(?:[a-f\\d]{6})$/i', $tempdata['bgcolor'])) {
        $mydata['showpassword'] = $tempdata['showpassword'];
        $mydata['bgcolor'] = $tempdata['bgcolor'];
        }
    }
    }
    return $mydata;
}
//This function retrieves data from cookies.
//Data in cookies are not the original data.
//Use json_decode(xor_encrypt(base64_decode($_COOKIE["data"])), true) to convert back.
// (?: ) means group, ?: means don't document the content
// \\d means 0-9, $ means the end of a string
```

Function `loadData()` didn’t check “`showpassward`”

```php
function saveData($d) {
    setcookie("data", base64_encode(xor_encrypt(json_encode($d))));
}

$data = loadData($defaultdata);

if(array_key_exists("bgcolor",$_REQUEST)) {
    if (preg_match('/^#(?:[a-f\\d]{6})$/i', $_REQUEST['bgcolor'])) {
        $data['bgcolor'] = $_REQUEST['bgcolor'];
    }
}

saveData($data);
```

```bash
* Request completely sent off
< HTTP/1.1 200 OK
< Date: Mon, 19 Jan 2026 12:46:44 GMT
< Server: Apache/2.4.58 (Ubuntu)
< Set-Cookie: data=HmYkBwozJw4WNyAAFyB1VUcqOE1JZjUIBis7ABdmbU1GIjEJAyIxTRg%3D
< Vary: Accept-Encoding
< Content-Length: 1085
< Content-Type: text/html; charset=UTF-8
```

base64 often come with = in the end, server or browser will do URL encoding automatically, and = become %3D

```bash
curl -v -u natas11:UJdqkK1pTu6VLt9UHWAgRZz6sVUZ3lEk <http://natas11.natas.labs.overthewire.org/> -H "Cookie: data={encrypted text}"
```