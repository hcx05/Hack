
---
[Link-Local Multicast Name Resolution](https://datatracker.ietf.org/doc/html/rfc4795) (LLMNR) and [NetBIOS Name Service](https://docs.microsoft.com/en-us/previous-versions/windows/it-pro/windows-2000-server/cc940063\(v=technet.10\)?redirectedfrom=MSDN) (NBT-NS) are Microsoft Windows components that serve as alternate methods of host identification that can be used when DNS fails.

LLMNR is based upon the Domain Name System (DNS) format and allows hosts on the same local link to perform name resolution for other hosts. It uses port `5355` over UDP natively.
### TTPs
Several tools can be used to attempt LLMNR & NBT-NS poisoning:

| **Tool**                                              | **Description**                                                                                     |
| ----------------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| [Responder](https://github.com/lgandx/Responder)      | Responder is a purpose-built tool to poison LLMNR, NBT-NS, and MDNS, with many different functions. |
| [Inveigh](https://github.com/Kevin-Robertson/Inveigh) | Inveigh is a cross-platform MITM platform that can be used for spoofing and poisoning attacks.      |
| [Metasploit](https://www.metasploit.com/)             | Metasploit has several built-in scanners and spoofing modules made to deal with poisoning attacks.  |
 Both Responder and Inveigh can be used to attack the following protocols:

- LLMNR
- DNS
- MDNS
- NBNS
- DHCP
- ICMP
- HTTP
- HTTPS
- SMB
- LDAP
- WebDAV
- Proxy Auth

Responder also has support for:

- MSSQL
- DCE-RPC
- FTP, POP3, IMAP, and SMTP auth
#### Responder
`Responder` is an `LLMNR`, `NBT-NS` and `MDNS` poisoner, with built-in HTTP/SMB/MSSQL/FTP/LDAP rogue authentication server supporting NTLMv1/NTLMv2/LMv2, Extended Security NTLMSSP, and Basic HTTP authentication.

`-A` flag puts us into analyze mode, allowing us to see NBT-NS, BROWSER, and LLMNR requests in the environment without poisoning any responses.

Responder will listen and answer any requests it sees on the wire. If you are successful and manage to capture a hash, Responder will print it out on screen and write it to a log file per host located in the `/usr/share/responder/logs` directory.

#### Pretender
In addition to `Responder`, [Pretender](https://github.com/RedTeamPentesting/pretender) is a new powerful tool written in Go, developed by [RedTeam Pentesting](https://blog.redteam-pentesting.de/2022/introducing-pretender/). It obtains a man-in-the-middle position by spoofing name resolutions and DHCPv6 DNS-related takeover attacks. `Pretender` can be executed without answering any name resolution using the `--dry` flag, which helps analyze the broadcast traffic in the environment. `Pretender` offers [releases](https://github.com/RedTeamPentesting/pretender/releases/tag/v1.1.1) ready for use. By default, it poisons DHCP requests, `pretender` can be highly disruptive in production networks.

**Note:** The problem with poisoning DHCP requests is that they will be semi-permanent on the machine until the request time has expired, the machine is rebooted, or a new DHCP request is forced. While this happens, any DNS requests made by this machine may miss their target, leaving the machine with network problems.














































