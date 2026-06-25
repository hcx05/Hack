
---
Featuring fantastic GPU support, it can be used to crack a large variety of hashes.
```bash
hashcat -a 0 -m 0 <hashes> [wordlist, rule, mask, ...]
```
- `-a` is used to specify the `attack mode`
- `-m` is used to specify the `hash type`
- `<hashes>` is a either a hash string, or a file containing one or more password hashes of the same type
- `[wordlist, rule, mask, ...]` is a placeholder for additional arguments that depend on the attack mode
### Hash types
[Example hashes](https://hashcat.net/wiki/doku.php?id=example_hashes)
```bash
hcx05@htb[/htb]$ hashid -m '$1$FNr44XZC$wQxY6HHLrgrGX0e1195k.1'
#-m: Hashcat format

Analyzing '$1$FNr44XZC$wQxY6HHLrgrGX0e1195k.1'
[+] MD5 Crypt [Hashcat Mode: 500]
[+] Cisco-IOS(MD5) [Hashcat Mode: 500]
[+] FreeBSD MD5 [Hashcat Mode: 500]
```
### Attack modes
#### Dictionary attack
[Dictionary attack](https://hashcat.net/wiki/doku.php?id=dictionary_attack) (`-a 0`) is, as the name suggests, a dictionary attack.
```bash
hcx05@htb[/htb]$ hashcat -a 0 -m 0 e3e3ec5831ad5e7288241960e5d4fdb8 /usr/share/wordlists/rockyou.txt

...SNIP...               

Session..........: hashcat
Status...........: Cracked
Hash.Mode........: 0 (MD5)
Hash.Target......: e3e3ec5831ad5e7288241960e5d4fdb8
Time.Started.....: Sat Apr 19 08:58:44 2025 (0 secs)
Time.Estimated...: Sat Apr 19 08:58:44 2025 (0 secs)
Kernel.Feature...: Pure Kernel
Guess.Base.......: File (/usr/share/wordlists/rockyou.txt)
Guess.Queue......: 1/1 (100.00%)
Speed.#1.........:  1706.6 kH/s (0.14ms) @ Accel:512 Loops:1 Thr:1 Vec:8
Recovered........: 1/1 (100.00%) Digests (total), 1/1 (100.00%) Digests (new)
Progress.........: 28672/14344385 (0.20%)
Rejected.........: 0/28672 (0.00%)
Restore.Point....: 27648/14344385 (0.19%)
Restore.Sub.#1...: Salt:0 Amplifier:0-1 Iteration:0-1
Candidate.Engine.: Device Generator
Candidates.#1....: 010292 -> spongebob9
Hardware.Mon.#1..: Util: 40%

Started: Sat Apr 19 08:58:43 2025
Stopped: Sat Apr 19 08:58:46 2025
```
Show cracked password
```bash
hashcat -m 0 e3e3ec5831ad5e7288241960e5d4fdb8 --show
```
The rule files that come with hashcat are typically found under `/usr/share/hashcat/rules`
```bash
hcx05@htb[/htb]$ hashcat -a 0 -m 0 1b0556a75770563578569ae21392630c /usr/share/wordlists/rockyou.txt -r /usr/share/hashcat/rules/best64.rule

...SNIP...

Session..........: hashcat
Status...........: Cracked
Hash.Mode........: 0 (MD5)
Hash.Target......: 1b0556a75770563578569ae21392630c
Time.Started.....: Sat Apr 19 09:16:35 2025 (0 secs)
Time.Estimated...: Sat Apr 19 09:16:35 2025 (0 secs)
Kernel.Feature...: Pure Kernel
Guess.Base.......: File (/usr/share/wordlists/rockyou.txt)
Guess.Mod........: Rules (/usr/share/hashcat/rules/best64.rule)
Guess.Queue......: 1/1 (100.00%)
Speed.#1.........: 13624.4 kH/s (5.40ms) @ Accel:512 Loops:77 Thr:1 Vec:8
Recovered........: 1/1 (100.00%) Digests (total), 1/1 (100.00%) Digests (new)
Progress.........: 236544/1104517645 (0.02%)
Rejected.........: 0/236544 (0.00%)
Restore.Point....: 2048/14344385 (0.01%)
Restore.Sub.#1...: Salt:0 Amplifier:0-77 Iteration:0-77
Candidate.Engine.: Device Generator
Candidates.#1....: slimshady -> drousd
Hardware.Mon.#1..: Util: 47%

Started: Sat Apr 19 09:16:35 2025
Stopped: Sat Apr 19 09:16:37 2025
```
#### Mask attack
|Symbol|Charset|
|---|---|
|?l|abcdefghijklmnopqrstuvwxyz|
|?u|ABCDEFGHIJKLMNOPQRSTUVWXYZ|
|?d|0123456789|
|?h|0123456789abcdef|
|?H|0123456789ABCDEF|
|?s|«space»!"#$%&'()*+,-./:;<=>?@[]^_`{|
|?a|?l?u?d?s|
|?b|0x00 - 0xff|
```bash
hcx05@htb[/htb]$ hashcat -a 3 -m 0 1e293d6912d074c0fd15844d803400dd '?u?l?l?l?l?d?s'

...SNIP...

Session..........: hashcat
Status...........: Cracked
Hash.Mode........: 0 (MD5)
Hash.Target......: 1e293d6912d074c0fd15844d803400dd
Time.Started.....: Sat Apr 19 09:43:02 2025 (4 secs)
Time.Estimated...: Sat Apr 19 09:43:06 2025 (0 secs)
Kernel.Feature...: Pure Kernel
Guess.Mask.......: ?u?l?l?l?l?d?s [7]
Guess.Queue......: 1/1 (100.00%)
Speed.#1.........:   101.6 MH/s (9.29ms) @ Accel:512 Loops:1024 Thr:1 Vec:8
Recovered........: 1/1 (100.00%) Digests (total), 1/1 (100.00%) Digests (new)
Progress.........: 456237056/3920854080 (11.64%)
Rejected.........: 0/456237056 (0.00%)
Restore.Point....: 25600/223080 (11.48%)
Restore.Sub.#1...: Salt:0 Amplifier:5120-6144 Iteration:0-1024
Candidate.Engine.: Device Generator
Candidates.#1....: Uayvf7- -> Dikqn5!
Hardware.Mon.#1..: Util: 98%

Started: Sat Apr 19 09:42:46 2025
Stopped: Sat Apr 19 09:43:08 2025
```

















