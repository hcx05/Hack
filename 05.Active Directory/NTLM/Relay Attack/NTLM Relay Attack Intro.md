
---
### MiTM Attack
Domain-joined computers:
![](assets/Pasted%20image%2020261020122214.png)
Workgruoup:
![](assets/Pasted%20image%2020261020122325.png)
### Phase I: Pre-relay
The `pre-relay` phase focuses on techniques that induce/coerce a client to initiate `NTLM` authentication for a service on a server.
![](assets/Pasted%20image%2020261020130115.png)
### Phase II: Relay
The `Relay` phase focuses on relaying the `NTLM` authentication of the client to a relay target.
#### Hunting for Relay Targets
We must find machines that meet some criteria: if we target `SMB` on the relay targets, we need `SMB` `signing` to be `disabled` (we will study other protocols in an upcoming section). We can use multiple tools to enumerate SMB `signing` settings, including `RunFinger.py`, `CrackMapExec`, and `Nmap`.
#### RunFinger
[RunFinger.py](https://github.com/lgandx/Responder/blob/master/tools/RunFinger.py) is a tool included with `Responder` (found in `/home/htb-students/tools/Responder/tools/`) that can be used to enumerate the network for hosts with SMB `signing` off, in addition to finding whether some standard services are running on the hosts:
```bash
hcx05@htb[/htb]$ python3 RunFinger.py -i 172.16.117.0/24

[SMB2]:['172.16.117.3', Os:'Windows 10/Server 2016/2019 (check build)', Build:'17763', Domain:'INLANEFREIGHT', Bootime: 'Unknown', Signing:'True', RDP:'True', SMB1:'False', MSSQL:'False']
<SNIP>
```
#### CrackMapExec
Another tool that we can use to find machines with `SMB signing` disabled is `CrackMapExec`; the option `--gen-relay-list FILE_NAME` generates a list of computers with signing disabled based on a given subnet and saves them into a file:
```bash
hcx05@htb[/htb]$ crackmapexec smb 172.16.117.0/24 --gen-relay-list relayTargets.txt

SMB         172.16.117.3    445    DC01             [*] Windows 10.0 Build 17763 x64 (name:DC01) (domain:INLANEFREIGHT.LOCAL) (signing:True) (SMBv1:False)
SMB         172.16.117.50   445    WS01             [*] Windows 10.0 Build 17763 x64 (name:WS01) (domain:INLANEFREIGHT.LOCAL) (signing:False) (SMBv1:False)
<SNIP>
Running CME against 256 targets ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 100% 0:00:00
```
#### Nmap
```bash
hcx05@htb[/htb]$ nmap -Pn --script=smb2-security-mode.nse -p 445 172.16.117.0/24 --open

Starting Nmap 7.80 ( https://nmap.org ) at 2023-07-30 20:39 UTC
Nmap scan report for dc01 (172.16.117.3)
Host is up (0.00034s latency).

PORT    STATE SERVICE
445/tcp open  microsoft-ds

Host script results:
| smb2-security-mode: 
|   2.02: 
|_    Message signing enabled and required

Nmap scan report for ws01 (172.16.117.50)
Host is up (0.00011s latency).

PORT    STATE SERVICE
445/tcp open  microsoft-ds

Host script results:
| smb2-security-mode: 
|   2.02: 
|_    Message signing enabled but not required

<SNIP>
```
### Phase III: Post-relay
The `post-relay` phase takes advantage of the authenticated session we obtained through relaying a victim's `NTLM` authentication. We can conduct specific post-relay attacks depending on the authenticated session's protocol.
### Attack phase mindmap
![](assets/Pasted%20image%2020261020124040.png)

| **Component**             | **Explanation**                                                                                                                                                                                                                                                                                                                                                                                                                     |
| :------------------------ | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Coercion Method`         | These techniques induce/coerce the client into performing authentication, including `AiTM` and `authentication coercion`.                                                                                                                                                                                                                                                                                                           |
| `Incoming NTLM auth over` | These are the protocols that the client uses to perform `NTLM` authentication, including `HTTP` (`1`) and `SMB` (`2`).                                                                                                                                                                                                                                                                                                              |
| `Client-side mitigation`  | These are the client-side mitigations that clients can have to protect against `NTLM` relay attacks, depending on the protocol specified in the `Incoming NTLM auth over` component.                                                                                                                                                                                                                                                |
| `Server-side mitigation`  | These are the server-side mitigations that servers can have to protect against `NTLM` relay attacks, depending on the protocol specified in the `Incoming NTLM auth over` component.                                                                                                                                                                                                                                                |
| `Relayed NTLM auth over`  | These are the protocols that we can relay the `NTLM` authentication received from the client over, including `HTTP`, `HTTPS`, `SMB`, `LDAP`, and `LDAPs`.                                                                                                                                                                                                                                                                           |
| `Post-relay attack`       | These are the post-relay attacks that we can carry out depending on the protocol of the `Relayed NTLM auth over` component. However, reaching this component depends on the two circles belonging to the `Client-side mitigation` and `Server-side mitigation` components. If both are green, the relay attack will work. However, if one is yellow, the relay attack will only work if the server suffers certain vulnerabilities. |


















