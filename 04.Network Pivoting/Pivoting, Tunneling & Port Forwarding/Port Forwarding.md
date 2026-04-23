
---
### Remote/Reverse Port Forwarding with ssh

---
SSH -R
```bash
ssh -R <InternalIPofPivotHost>:8080:0.0.0.0:8000 ubuntu@<ipAddressofTarget> -vN
#Telling ssh to open a listening port, which is port 8080 on 172.16.5.129
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