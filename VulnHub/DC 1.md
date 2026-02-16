
---
```bash
nmap -sV -sC -Pn 192.168.56.104                                                                                
Starting Nmap 7.98 ( https://nmap.org ) at 2026-02-15 09:26 -0500
Nmap scan report for 192.168.56.104
Host is up (0.000077s latency).
Not shown: 997 closed tcp ports (reset)
PORT    STATE SERVICE VERSION
22/tcp  open  ssh     OpenSSH 6.0p1 Debian 4+deb7u7 (protocol 2.0)
| ssh-hostkey: 
|   1024 c4:d6:59:e6:77:4c:22:7a:96:16:60:67:8b:42:48:8f (DSA)
|   2048 11:82:fe:53:4e:dc:5b:32:7f:44:64:82:75:7d:d0:a0 (RSA)
|_  256 3d:aa:98:5c:87:af:ea:84:b8:23:68:8d:b9:05:5f:d8 (ECDSA)
80/tcp  open  http    Apache httpd 2.2.22 ((Debian))
| http-robots.txt: 36 disallowed entries (15 shown)
| /includes/ /misc/ /modules/ /profiles/ /scripts/ 
| /themes/ /CHANGELOG.txt /cron.php /INSTALL.mysql.txt 
| /INSTALL.pgsql.txt /INSTALL.sqlite.txt /install.php /INSTALL.txt 
|_/LICENSE.txt /MAINTAINERS.txt
|_http-generator: Drupal 7 (http://drupal.org)
|_http-title: Welcome to Drupal Site | Drupal Site
|_http-server-header: Apache/2.2.22 (Debian)
111/tcp open  rpcbind 2-4 (RPC #100000)
| rpcinfo: 
|   program version    port/proto  service
|   100000  2,3,4        111/tcp   rpcbind
|   100000  2,3,4        111/udp   rpcbind
|   100000  3,4          111/tcp6  rpcbind
|   100000  3,4          111/udp6  rpcbind
|   100024  1          36654/udp6  status
|   100024  1          46076/tcp6  status
|   100024  1          56495/tcp   status
|_  100024  1          57166/udp   status
MAC Address: 08:00:27:E4:96:D8 (Oracle VirtualBox virtual NIC)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 8.92 seconds

```

```bash
gobuster dir -u http://192.168.56.104 -w /home/kali/CyberSec/SecLists/Discovery/Web-Content/DirBuster-2007_directory-list-lowercase-2.3-small.txt
===============================================================
Gobuster v3.8.2
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://192.168.56.104
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /home/kali/CyberSec/SecLists/Discovery/Web-Content/DirBuster-2007_directory-list-lowercase-2.3-small.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.8.2
[+] Timeout:                 10s
===============================================================
Starting gobuster in directory enumeration mode
===============================================================
search               (Status: 403) [Size: 7563]
misc                 (Status: 301) [Size: 315] [--> http://192.168.56.104/misc/]
themes               (Status: 301) [Size: 317] [--> http://192.168.56.104/themes/]
0                    (Status: 200) [Size: 7627]
user                 (Status: 200) [Size: 7480]
modules              (Status: 301) [Size: 318] [--> http://192.168.56.104/modules/]
admin                (Status: 403) [Size: 7718]
scripts              (Status: 301) [Size: 318] [--> http://192.168.56.104/scripts/]
node                 (Status: 200) [Size: 7627]
sites                (Status: 301) [Size: 316] [--> http://192.168.56.104/sites/]
includes             (Status: 301) [Size: 319] [--> http://192.168.56.104/includes/]
profiles             (Status: 301) [Size: 319] [--> http://192.168.56.104/profiles/]
robots               (Status: 200) [Size: 1561]
Progress: 10771 / 81642 (13.19%)
```

rpcinfo : Makes an RPC call to an RPC server and reports what it finds.

-p : Display a  list of all registered RPC programs.

```bash
rpcinfo -p 192.168.56.104
   program vers proto   port  service
    100000    4   tcp    111  portmapper
    100000    3   tcp    111  portmapper
    100000    2   tcp    111  portmapper
    100000    4   udp    111  portmapper
    100000    3   udp    111  portmapper
    100000    2   udp    111  portmapper
    100024    1   udp  57166  status
    100024    1   tcp  56495  status

```

No nfs(Network File System) exist, `rpcinfo` is not the main breakthrough.

```bash
curl -I 192.168.56.104
HTTP/1.1 200 OK
Date: Sun, 15 Feb 2026 16:12:17 GMT
Server: Apache/2.2.22 (Debian)
X-Powered-By: PHP/5.4.45-0+deb7u14
Expires: Sun, 19 Nov 1978 05:00:00 GMT
Last-Modified: Sun, 15 Feb 2026 16:12:17 +0000
Cache-Control: no-cache, must-revalidate, post-check=0, pre-check=0
ETag: "1771171937"
Content-Language: en
X-Generator: Drupal 7 (http://drupal.org)
Vary: Accept-Encoding
Content-Type: text/html; charset=utf-8

```

```bash
searchsploit Drupal 7
-------------------------------------------------------------- ---------------------------------
 Exploit Title                                                |  Path
-------------------------------------------------------------- ---------------------------------
Drupal 10.1.2 - web-cache-poisoning-External-service-interact | php/webapps/51723.txt
Drupal 4.1/4.2 - Cross-Site Scripting                         | php/webapps/22940.txt
Drupal 4.5.3 < 4.6.1 - Comments PHP Injection                 | php/webapps/1088.pl
Drupal 4.7 - 'Attachment mod_mime' Remote Command Execution   | php/webapps/1821.php
Drupal 4.x - URL-Encoded Input HTML Injection                 | php/webapps/27020.txt
Drupal 5.2 - PHP Zend Hash ation Vector                       | php/webapps/4510.txt
Drupal 6.15 - Multiple Persistent Cross-Site Scripting Vulner | php/webapps/11060.txt
Drupal 7.0 < 7.31 - 'Drupalgeddon' SQL Injection (Add Admin U | php/webapps/34992.py
Drupal 7.0 < 7.31 - 'Drupalgeddon' SQL Injection (Admin Sessi | php/webapps/44355.php
Drupal 7.0 < 7.31 - 'Drupalgeddon' SQL Injection (PoC) (Reset | php/webapps/34984.py
Drupal 7.0 < 7.31 - 'Drupalgeddon' SQL Injection (PoC) (Reset | php/webapps/34993.php
Drupal 7.0 < 7.31 - 'Drupalgeddon' SQL Injection (Remote Code | php/webapps/35150.php
Drupal 7.12 - Multiple Vulnerabilities                        | php/webapps/18564.txt
Drupal 7.x Module Services - Remote Code Execution            | php/webapps/41564.php
Drupal < 4.7.6 - Post Comments Remote Command Execution       | php/webapps/3313.pl
Drupal < 5.1 - Post Comments Remote Command Execution         | php/webapps/3312.pl
Drupal < 5.22/6.16 - Multiple Vulnerabilities                 | php/webapps/33706.txt
Drupal < 7.34 - Denial of Service                             | php/dos/35415.txt
Drupal < 7.58 - 'Drupalgeddon3' (Authenticated) Remote Code ( | php/webapps/44557.rb
Drupal < 7.58 - 'Drupalgeddon3' (Authenticated) Remote Code ( | php/webapps/44557.rb
Drupal < 7.58 - 'Drupalgeddon3' (Authenticated) Remote Code E | php/webapps/44542.txt
Drupal < 7.58 / < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' | php/webapps/44449.rb
Drupal < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote C | php/remote/44482.rb
Drupal < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote C | php/remote/44482.rb
Drupal < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote C | php/webapps/44448.py
Drupal < 8.5.11 / < 8.6.10 - RESTful Web Services unserialize | php/remote/46510.rb
Drupal < 8.5.11 / < 8.6.10 - RESTful Web Services unserialize | php/remote/46510.rb
Drupal < 8.6.10 / < 8.5.11 - REST Module Remote Code Executio | php/webapps/46452.txt
Drupal < 8.6.9 - REST Module Remote Code Execution            | php/webapps/46459.py
Drupal avatar_uploader v7.x-1.0-beta8 - Arbitrary File Disclo | php/webapps/44501.txt
Drupal avatar_uploader v7.x-1.0-beta8 - Cross Site Scripting  | php/webapps/50841.txt
Drupal Module CKEditor < 4.1WYSIWYG (Drupal 6.x/7.x) - Persis | php/webapps/25493.txt
Drupal Module CODER 2.5 - Remote Command Execution (Metasploi | php/webapps/40149.rb
Drupal Module Coder < 7.x-1.3/7.x-2.6 - Remote Code Execution | php/remote/40144.php
Drupal Module Cumulus 5.x-1.1/6.x-1.4 - 'tagcloud' Cross-Site | php/webapps/35397.txt
Drupal Module Drag & Drop Gallery 6.x-1.5 - 'upload.php' Arbi | php/webapps/37453.php
Drupal Module Embedded Media Field/Media 6.x : Video Flotsam/ | php/webapps/35072.txt
Drupal Module RESTWS 7.x - PHP Remote Code Execution (Metaspl | php/remote/40130.rb
Drupal Module Sections - Cross-Site Scripting                 | php/webapps/10485.txt
Drupal Module Sections 5.x-1.2/6.x-1.2 - HTML Injection       | php/webapps/33410.txt
-------------------------------------------------------------- ---------------------------------
Shellcodes: No Results

```

Drupalgeddon SQL Injection : 
	`?q=` query parameter was historically used to pass the internal system path to the main `index.php` file, which then processed the request. For example, a request for the page "about-us" would internally be processed as `index.php?q=about-us`.This parameter is a technical detail of how Drupal 7 and earlier versions handled URL routing, especially when "Clean URLs" were not enabled.