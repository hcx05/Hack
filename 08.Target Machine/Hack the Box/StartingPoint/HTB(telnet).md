
---
```jsx
nmap -sV -Pn -p- --min-rate 5000 {target_IP}
//-sV: Probe open ports to determine service/version info
```

Identified port 23/tcp in an open state, running the telnet service.

VPN : Virtual Private Network

telnet : An old service used for remote management of other hosts on the network. User name ‘root’ is able to log into the target over telnet with a blank password. Some typical important accounts have self-explanatory names, such as : admin, administrator, root.(These user usually have the highest permission)

```jsx
telnet {target_IP}
```
