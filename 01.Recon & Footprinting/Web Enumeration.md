
---
Add a DNS Server such as 1.1.1.1 and 8.8.8.8 to `/etc/resolv.conf`.
```vim
nameserver 1.1.1.1
nameserver 8.8.8.8
```

### Banner Grabbing/ Web server Headers
```bash
curl -IL httpe://[...]
#-L : Fellow the redirection.
```
Extract the version of web servers, supporting frameworks, and applications using the command-line tool `whatweb`.
```bash
hcx05@htb[/htb]$ whatweb 10.10.10.121
#Use -a 3 or 4 to preform a more aggressive scan.

http://10.10.10.121 [200 OK] Apache[2.4.41], Country[RESERVED][ZZ], Email[license@php.net], HTTPServer[Ubuntu Linux][Apache/2.4.41 (Ubuntu)], IP[10.10.10.121], Title[PHP 7.4.3 - phpinfo()]
```
