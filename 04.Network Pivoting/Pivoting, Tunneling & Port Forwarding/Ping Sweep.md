
---
Recon alive hosts
### Using ICMP Protocol

---
```bash
meterpreter > run post/multi/gather/ping_sweep RHOSTS=172.16.5.0/23 

[*] Performing ping sweep for IP range 172.16.5.0/23

#Note: It is possible that a ping sweep may not result in successful replies on the first attempt, especially when communicating across networks. This can be caused by the time it takes for a host to build its ARP(Address Resolution Protocol) cache. In these cases, it is good to attempt our ping sweep at least twice to ensure the arp cache gets built.
```

The downside of `ping_sleep`: Nowadays, most of the Windows hosts have Endpoint Firewall and will discard ICMP package.

#### Using For Loop on Linux Pivot Hosts
```bash
for i in {1..254} ;do (ping -c 1 172.16.5.$i | grep "bytes from" &) ;done
```

#### Using For Loop Using CMD
```bash
for /L %i in (1 1 254) do ping 172.16.5.%i -n 1 -w 100 | find "Reply"
```

#### Using PowerShell
```powershell
1..254 | % {"172.16.5.$($_): $(Test-Connection -count 1 -comp 172.16.5.$($_) -quiet)"}
```

### Using TCP Protocol

---
On our own attack box

#### Port Forwarding using `socks_proxy`
```bash
msf > use auxiliary/server/socks_proxy msf
auxiliary(server/socks_proxy) > set SRVPORT 9050 SRVPORT => 9050 msf auxiliary(server/socks_proxy) > set SRVHOST 0.0.0.0 SRVHOST => 0.0.0.0 msf auxiliary(server/socks_proxy) > set version 4a version => 4a msf auxiliary(server/socks_proxy) > run 
[*] Auxiliary module running as background job 0. 

[*] Starting the SOCKS proxy server 
msf auxiliary(server/socks_proxy) > options 
Module options (auxiliary/server/socks_proxy): 
Name Current Setting Required Description 
---- --------------- -------- ----------- 
SRVHOST 0.0.0.0 yes The address to listen on 
SRVPORT 9050 yes The port to listen on 
VERSION 4a yes The SOCKS version to use (Accepted: 4a, 5) 

Auxiliary action: 
Name  Description 
----  ----------- 
Proxy Run a SOCKS proxy server

```

#### Confirming Proxy Server is Running
```bash
msf auxiliary(server/socks_proxy) > jobs
```

#### Adding a Line to proxychains.conf if Needed
```bash
socks4 127.0.0.1 9050
```

#### Creating Routes
```bash
msf > route add 172.16.5.0 255.255.254.0 [SESSION ID]
```
Or
```bash
meterpreter > run autoroute -s 172.16.5.0/23
```

#### Checking 
```bash
meterpreter > run autoroute -p 

[!] Meterpreter scripts are deprecated. Try post/multi/manage/autoroute. [!] Example: run post/multi/manage/autoroute OPTION=value [...] 
Active Routing Table 
==================== 

Subnet Netmask Gateway 
------ ------- ------- 
10.129.0.0 255.255.0.0 Session 1 
172.16.4.0 255.255.254.0 Session 1 
172.16.5.0 255.255.254.0 Session 1
```

#### Start Scanning
```bash
proxychains nmap -sT -Pn -p 445 172.16.5.0/23
```


### Using Address Resolution Protocol

---
#### Checking the history connections.
```bash
arp -a
```
