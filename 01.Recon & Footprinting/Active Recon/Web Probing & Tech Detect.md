
---
### Whatweb
Extract the version of web servers, supporting frameworks, and applications using the command-line tool `whatweb`.
```bash
hcx05@htb[/htb]$ whatweb 10.10.10.121
#Use -a 3 or 4 to preform a more aggressive scan.

http://10.10.10.121 [200 OK] Apache[2.4.41], Country[RESERVED][ZZ], Email[license@php.net], HTTPServer[Ubuntu Linux][Apache/2.4.41 (Ubuntu)], IP[10.10.10.121], Title[PHP 7.4.3 - phpinfo()]
```

### httpx
Checking the alive services
```bash
cat subdomains.txt | httpx -sc -title -td -ip -o alive_sites.txt
```

And find vulnerable targers
```bash
cat alive_sites.txt | grep -E "dev|test|admin|api|git"
```

### Screenshot Analysis

### Scanning
Use `Nuclei` to find existing CVE
```bash
cat alive_sites.txt | nuclei -t critical -t high
```

---
Passive Banner Grabbing go to [Domain & DNS](../Passive%20Recon/Domain%20&%20DNS.md).
### Banner Grabbing/ Web server Headers
Add a DNS Server such as 1.1.1.1 and 8.8.8.8 to `/etc/resolv.conf`.

```vim
nameserver 1.1.1.1
nameserver 8.8.8.8
```

```bash
curl -IL https://[...]
#-L : Fellow the redirection.
```
