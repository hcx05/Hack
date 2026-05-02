
---

### Error code
|**Code**|**Status**|**Meaning & Strategic Inference**|
|---|---|---|
|**0**|**Net Unreachable**|**The path is gone.** The router has no route to the target network. Usually a routing configuration error or a network link is down.|
|**1**|**Host Unreachable**|**The "house" is gone.** The router can find the network, but the specific target IP address is not responding (no ARP response). The host might be offline.|
|**2**|**Proto Unreachable**|**The "language" is unknown.** The target host exists, but it doesn't support the protocol you used (e.g., you sent a GRE packet to a host that only understands TCP/UDP).|
|**3**|**Port Unreachable**|**The "door" is closed.** The packet reached the target host, but there is no service listening on that port. **Note:** This is the "honest" response you get from a target with no firewall blocking you.|
|**9**|**Net Prohibited**|**The "border" is closed.** The network is blocked by an administrative rule (ACL). Often seen on border routers.|
|**10**|**Host Prohibited**|**The "VIP" is blocked.** The specific host is blocked by an administrative rule, even if the rest of the network is accessible.|
|**13**|**Communication Prohibited**|**The "Guard" stopped you.** This is the classic **Firewall (ACL)** response. The administrator has explicitly set a rule to "Reject" your traffic.|

ACK-Scan is useful to prob the defense mechanism.

### Decoys
With this method, Nmap generates various random IP addresses inserted into the IP header to disguise the origin of the packet sent.
```bash
hcx05@htb[/htb]$ sudo nmap 10.129.2.28 -p 80 -sS -Pn -n --disable-arp-ping --packet-trace -D RND:5 

Starting Nmap 7.80 ( https://nmap.org ) at 2020-06-21 16:14 CEST SENT (0.0378s) TCP 102.52.161.59:59289 > 10.129.2.28:80 S ttl=42 id=29822 iplen=44 seq=3687542010 win=1024 <mss 1460> 
SENT (0.0378s) TCP 10.10.14.2:59289 > 10.129.2.28:80 S ttl=59 id=29822 iplen=44 seq=3687542010 win=1024 <mss 1460> 
SENT (0.0379s) TCP 210.120.38.29:59289 > 10.129.2.28:80 S ttl=37 id=29822 iplen=44 seq=3687542010 win=1024 <mss 1460> 
SENT (0.0379s) TCP 191.6.64.171:59289 > 10.129.2.28:80 S ttl=38 id=29822 iplen=44 seq=3687542010 win=1024 <mss 1460> 
SENT (0.0379s) TCP 184.178.194.209:59289 > 10.129.2.28:80 S ttl=39 id=29822 iplen=44 seq=3687542010 win=1024 <mss 1460> 
SENT (0.0379s) TCP 43.21.121.33:59289 > 10.129.2.28:80 S ttl=55 id=29822 iplen=44 seq=3687542010 win=1024 <mss 1460> 
RCVD (0.1370s) TCP 10.129.2.28:80 > 10.10.14.2:59289 SA ttl=64 id=0 iplen=44 seq=4056111701 win=64240 <mss 1460> 
Nmap scan report for 10.129.2.28 
Host is up (0.099s latency).
```

### Scan by Using Different Source IP
```bash
sudo nmap 10.129.2.28 -n -Pn -p 445 -O -S 10.129.2.200 -e tun0
```

### DNS Proxying
```bash
sudo nmap 10.129.2.28 -p50000 -sS -Pn -n --disable-arp-ping --packet-trace --source-port 53
```
