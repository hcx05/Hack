
---
### Online Presence
- Use google or other browser `site:example.com`
- Check TLS certificate
- Find more subdomains in  [crt.sh](https://crt.sh/). this source is Certificate Transparency logs.
```bash
sudo apt update && sudo apt install jq -y
```

```bash
curl -s "https://crt.sh/?q=target.com&output=json" | jq -r '.[].name_value' | sed 's/\\n/\n/g' | sed 's/\*\.//g' | sort -u
#-s: silent
#jq: Use jq to parse json
#-r: Output original string, e.g. without ""
#'.[].name_value': A filter that iterates through every object in the JSON array (.[]) and extracts only the value associated with the key `name_value`.
#'s/\\n/\n/g': Handles multi-domain entries within a single JSON field.
#'s/\*\.//g' : Cleans up Wildcard Certificates.
```

Or use `subfinder`
```bash
sudo apt update && sudo apt install subfinder -y

subfinder -d target.com | tee subdomains.txt
```

#### Company Hosted Servers
Identify the hosts directly accessible from the Internet and not hosted by third-party providers. This is because we are not allowed to test(aka hack) the hosts without the permission of third-party providers.
```bash
for i in $(cat subdomainlist);do host $i | grep "has address" | grep target.com | cut -d" " -f1,4;done
#-f1,4: Only display row 1 and 4.
```

[Web Probing & Tech Detect](../Active%20Recon/Web%20Probing%20&%20Tech%20Detect.md).

### Shodan Banner Grabbing

---
```bash
for i in $(cat alive_sites.txt);do host $i | grep "has address" | grep inlanefreight.com | cut -d" " -f4 >> ip-addresses.txt;done

for i in $(cat ip-addresses.txt);do shodan host $i;done
```

### DNS Reconnaissance

---
```bash
hcx05@htb[/htb]$ dig any [target.com] 
# Nowadays many DNS server will ban any request 
# Add +trace to observe if there is a DNS Hijacking

; <<>> DiG 9.16.1-Ubuntu <<>> any inlanefreight.com 
;; global options: +cmd 
;; Got answer: 
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 52058 
;; flags: qr rd ra; QUERY: 1, ANSWER: 17, AUTHORITY: 0, ADDITIONAL: 1 

;; OPT PSEUDOSECTION: 
; EDNS: version: 0, flags:; udp: 65494 
;; QUESTION SECTION: 
;inlanefreight.com. IN ANY 

;; ANSWER SECTION: 
inlanefreight.com. 300 IN A 10.129.27.33 
inlanefreight.com. 300 IN A 10.129.95.250 
inlanefreight.com. 3600 IN MX 1 aspmx.l.google.com. 
inlanefreight.com. 3600 IN MX 10 aspmx2.googlemail.com. inlanefreight.com. 3600 IN MX 10 aspmx3.googlemail.com. inlanefreight.com. 3600 IN MX 5 alt1.aspmx.l.google.com. inlanefreight.com. 3600 IN MX 5 alt2.aspmx.l.google.com.
inlanefreight.com. 21600 IN NS ns.inwx.net.

<SNIP>

inlanefreight.com. 21600 IN SOA ns.inwx.net. hostmaster.inwx.net.
```

- `NS` records: These kinds of records show which name servers are used to resolve the FQDN to IP addresses. Most hosting providers use their own name servers, making it easier to identify the hosting provider.
	FQDN: Fully Qualified Domain Name

- `TXT` records: this type of record often contains verification keys for different third-party providers and other security aspects of DNS
```bash
...SNIP... TXT "MS=ms92346782372" 
...SNIP... TXT "atlassian-domain-verification=IJdXMt1rKCy68JFszSdCKVpwPN" 

...SNIP... TXT "google-site-verification=O7zV5-xFh_jn7JQ31" ...SNIP... TXT "google-site-verification=bow47-er9LdgoUeah" ...SNIP... TXT "google-site-verification=gZsCG-BINLopf4hr2" ...SNIP... TXT "logmein-verification-code=87123gff5a479e-61d4325gddkbvc1-b2bnfghfsed1-3c789427sdjirew63fc" 

...SNIP... TXT "v=spf1 include:mailgun.org include:_spf.google.com include:spf.protection.outlook.com include:_spf.atlassian.net ip4:10.129.24.8 ip4:10.129.27.2 ip4:10.72.82.106 ~all"
```












