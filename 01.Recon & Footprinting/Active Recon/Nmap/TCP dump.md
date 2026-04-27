
---
PSH is the flag in TCP layer, which enable server push the data an the cache to client automatically.

### Grabing the banner using nc
```bash

hcx05@htb[/htb]$ sudo tcpdump -i eth0 -X host 10.10.14.2 and 10.129.2.28
#-X: print the packet content in ASCII

tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth0, link-type EN10MB (Ethernet), capture size 262144 bytes

```

```bash
hcx05@htb[/htb]$ nc -nv 10.129.2.28 25 

Connection to 10.129.2.28 port 25 [tcp/*] succeeded! 
220 inlane ESMTP Postfix (Ubuntu)
```