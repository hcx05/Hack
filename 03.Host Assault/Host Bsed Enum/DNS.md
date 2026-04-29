
---
DNS does not have a central datebase.

#### Types of DNS server
`DNS Root Server`: The root servers of the DNS are responsible for the top-level domains (`TLD`). As the last instance, they
are only requested if the name server does not respond.

`Authoritative Nameserver`: They only answer queries from their area of responsibility, and their information is binding. If an authoritative name server cannot answer a client's query, the root name server takes over at that point.

`Non-authoritative Nameserver`: Non-authoritative name servers are not responsible for a particular DNS zone. Instead, they collect information on specific DNS zones themselves, which is done using recursive or iterative DNS querying.

`Caching DNS Server`: Caching DNS servers cache information from other name servers for a specified period. The authoritative name server determines the duration(TTL) of this storage.

`Forwarding Server`: Forwarding DNS queries to another DNS server.

`Resolver`: Resolvers are not authoritative DNS servers but perform name resolution locally in the computer or router.

#### DNS Record
`A`: Returns an IPv4 address of the requested domain as a result.

`AAAA`: Returns an IPv6 address of the requested domain.

`MX`: Returns the responsible mail servers as a result.

`NS`: Returns the DNS servers (nameservers) of the domain.

`TXT`: This record can contain various information. The all-rounder can be used, e.g., to validate the Google Search Console or validate SSL certificates. In addition, SPF and DMARC entries are set to validate mail traffic and protect it from spam.

`CNAME`: This record serves as an alias for another domain name. 

`PTR`: It converts IP addresses into valid domain names.

`SOA`: Provides information about the corresponding DNS zone and email address of the administrative contact.

### Default Configuration
All DNS servers work with three different types of configuration files:

	1. local DNS configuration files
	2. zone files
	3. reverse name resolution files

#### Bind9
The DNS server [Bind9](https://www.isc.org/bind/) is very often used on Linux-based distributions. Its local configuration file (`named.conf`) is roughly divided into two sections, firstly the options section for general settings and secondly the zone entries for the individual domains. The local configuration files are usually:

- `named.conf.local`
- `named.conf.options`
- `named.conf.log`

#### Locak DNS Config
```bash
root@bind9:~# cat /etc/bind/named.conf.local

//
// Do any local configuration here
//

// Consider adding the 1918 zones here, if they are not used in your
// organization
//include "/etc/bind/zones.rfc1918";
zone "domain.com" {
    type master;
    file "/etc/bind/db.domain.com";
    allow-update { key rndc-key; };
};
```

#### Zone Files
```bash
cat /etc/bind/db.domain.com 
```

#### Reverse Name Resolution Zone Files
For the `Fully Qualified Domain Name` (`FQDN`) to be resolved from the IP address, the DNS server must have a reverse lookup file.
```bash
root@bind9:~# cat /etc/bind/db.10.129.14

;
; BIND reverse data file for local loopback interface
;
$ORIGIN 14.129.10.in-addr.arpa
$TTL 86400
@     IN     SOA    dns1.domain.com.     hostmaster.domain.com. (
                    2001062501 ; serial
                    21600      ; refresh after 6 hours
                    3600       ; retry after 1 hour
                    604800     ; expire after 1 week
                    86400 )    ; minimum TTL of 1 day

      IN     NS     ns1.domain.com.
      IN     NS     ns2.domain.com.

5    IN     PTR    server1.domain.com.
7    IN     MX     mx.domain.com.
...SNIP...
```

### Dangerous Settings
A list of vulnerabilities targeting the BIND9 server can be found at [CVEdetails](https://www.cvedetails.com/product/144/ISC-Bind.html?vendor_id=64). In addition, SecurityTrails provides a short [list](https://web.archive.org/web/20250329174745/https://securitytrails.com/blog/most-popular-types-dns-attacks) of the most popular attacks on DNS servers.

`allow-query`: Defines which hosts are allowed to send requests to the DNS server.

`allow-recursion`: Defines which hosts are allowed to send recursive requests to the DNS server.

`allow-transfer`: Defines which hosts are allowed to receive zone transfers from the DNS server.

`zone-statistics`: Collects statistical data of zones.

### Footprinting
#### NS Query
```bash
dig ns <domain> @<target_ip>
#This is a UDP request, port 53
#Use +tcp parameter if no responses
```

#### Version Query
Sometimes it is also possible to query a DNS server's version using a class CHAOS query and type TXT. However, this entry must exist on the DNS server.
```bash
dig CH TXT version.bind @<target_ip>
```

#### AXFR Zone Transfer
AXFR: Asynchronous Full Transfer Zone
```bash
dig axfr inlanefreight.htb @10.129.14.128
```

If the administrator used a subnet for the `allow-transfer` 
option for testing purposes or as a workaround solution or set it to `any`, everyone would query the entire zone file at the DNS server. In addition, other zones can be queried, which may even show internal IP addresses and hostnames.
```bash
dig axfr internal.inlanefreight.htb @10.129.14.128
```

#### Subdomain Brute Forcing
```bash
for sub in $(cat <subdomain_wordlist>);do dig $sub.example.com @10.129.14.128 | grep -v ';\|SOA' | sed -r '/^\s*$/d' | grep $sub | tee -a subdomains.txt;done
```

Or  [DNSenum](https://github.com/fwaeytens/dnsenum)
Perform `dig ns` + `dig axfr` + `Brute Force` + `Reverse Lookup` adll in once.
```bash
dnsenum --dnsserver 10.129.14.128 --enum -p 0 -s 0 -o subdomains.txt -f <subdonaim_wordlist> example.com
#-p 0: No google searching
#-s 0: No open source searching
```
















