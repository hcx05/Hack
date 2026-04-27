
---
### Host scanning
- -sn: 
	No Port scanning
- -PE(ICMP Echo Request):
	- Standerd `ping`, Sent Type 8(Echo Request), hoping for Type 0(Echo Reply).
	- Often been block by Firewall.
- -PP(ICMP Timestamp Request):
	- Sent Type 13(Time stamp Request), hoping for Type 14(System time reply), then we can confirmed that the host was alive indirectly.
- -PM(ICMP Address Mask Request): 
	- Sent Type 17(Subnet masking Request), most of the system, usually Linux, won't reply this request for security reason. But in some old or misconfigurated system, it can bypass the Firewall.

Add `--disable-arp-ping` or `--send-ip` to sent the ICMP request in the same Intranet.

Add `--packet-trace` or `--reason` to observe the detail communication.

`--stats-every=5s` is another option.

### Port scanning
#### Discovering Open TCP Ports
```bash
hcx05@htb[/htb]$ sudo nmap 10.129.2.28 -p 21 --packet-trace -Pn -n --disable-arp-ping 
#-n: Disables DNS resolution.

Starting Nmap 7.80 ( https://nmap.org ) at 2020-06-15 15:39 CEST 

SENT (0.0429s) TCP 10.10.14.2:63090 > 10.129.2.28:21 S ttl=56 id=57322 iplen=44 seq=1699105818 win=1024 <mss 1460> 
#S: SYN flag of the TCP packet.

RCVD (0.0573s) TCP 10.129.2.28:21 > 10.10.14.2:63090 RA ttl=64 id=0 iplen=40 seq=0 win=0 Nmap scan report for 10.129.2.28 
#RA: RST and ACK flags of the sent TCP packet.

Host is up (0.014s latency). 

PORT STATE SERVICE 
21/tcp closed ftp 
MAC Address: DE:AD:00:00:BE:EF (Intel Corporate) 
Nmap done: 1 IP address (1 host up) scanned in 0.07 seconds
```


#### Discovering Open UDP Ports
Since `UDP` is a `stateless protocol` and does not require a three-way handshake like TCP. We do not receive any acknowledgment.

`-sU`: UDP scan
```bash
hcx05@htb[/htb]$ sudo nmap 10.129.2.28 -F -sU 

Starting Nmap 7.80 ( https://nmap.org ) at 2020-06-15 16:01 CEST

Nmap scan report for 10.129.2.28 
Host is up (0.059s latency). 
Not shown: 95 closed ports 

PORT STATE SERVICE 
68/udp open|filtered dhcpc 
137/udp open netbios-ns 
138/udp open|filtered netbios-dgm 
631/udp open|filtered ipp 
5353/udp open zeroconf 
MAC Address: DE:AD:00:00:BE:EF (Intel Corporate) 

Nmap done: 1 IP address (1 host up) scanned in 98.07 seconds
```

Another disadvantage of this is that we often do not get a response back because `Nmap` sends empty datagrams to the scanned UDP ports, and we do not receive any response. So we cannot determine if the UDP packet has arrived at all or not. If the UDP port is `open`, we only get a response if the application is configured to do so.
```bash
hcx05@htb[/htb]$ sudo nmap 10.129.2.28 -sU -Pn -n --disable-arp-ping --packet-trace -p 137 --reason 

Starting Nmap 7.80 ( https://nmap.org ) at 2020-06-15 16:15 CEST 

SENT (0.0367s) UDP 10.10.14.2:55478 > 10.129.2.28:137 ttl=57 id=9122 iplen=78 RCVD (0.0398s) UDP 10.129.2.28:137 > 10.10.14.2:55478 ttl=64 id=13222 iplen=257 

Nmap scan report for 10.129.2.28 
Host is up, received user-set (0.0031s latency). 
PORT STATE SERVICE REASON 
137/udp open netbios-ns udp-response ttl 64 
MAC Address: DE:AD:00:00:BE:EF (Intel Corporate) 

Nmap done: 1 IP address (1 host up) scanned in 0.04 seconds
```
`udp-response`: open
`no-response`: Firewall or packate post


















