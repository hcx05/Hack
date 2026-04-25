
---
URL : Uniform Resources Locator

Requests and Responses

```http
GET / HTTP/1.1

Host: tryhackme.com
User-Agent: Mozilla/5.0 Firefox/87.0
Referer: https://tryhackme.com/
```

**Line 1:** This request is sending the GET method ( more on this in the HTTP Methods task ), request the home page with / and telling the web server we are using HTTP protocol version 1.1.
**Line 2:** We tell the web server we want the website tryhackme.com
**Line 3:** We tell the web server we are using the Firefox version 87 Browser
**Line 4:** We are telling the web server that the web page that referred us to this one is [https://tryhackme.com](https://tryhackme.com)

```http
HTTP/1.1 200 OK

Server: nginx/1.15.8
Date: Fri, 09 Apr 2021 13:34:03 GMT
Content-Type: text/html
Content-Length: 98


<html>
<head>
    <title>TryHackMe</title>
</head>
<body>
    Welcome To TryHackMe.com
</body>
</html>
```

**Line 1:** HTTP 1.1 is the version of the HTTP protocol the server is using and then followed by the HTTP Status Code in this case "200 OK" which tells us the request has completed successfully.
**Line 2:** This tells us the web server software and version number.
**Line 3:** The current date, time and timezone of the web server.
**Line 4:** The Content-Type header tells the client what sort of information is going to be sent, such as HTML, images, videos, pdf, XML.
**Line 5:** Content-Length tells the client how long the response is, this way we can confirm no data is missing.
**Line 6:** HTTP response contains a blank line to confirm the end of the HTTP response.
**Lines 7-14:** The information that has been requested, in this instance the homepage.

Herders : Contain extra information to give to the web server you’re communicating with
