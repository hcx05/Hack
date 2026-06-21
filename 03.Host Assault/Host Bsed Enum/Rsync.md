
---
This [guide](https://hacktricks.wiki/en/network-services-pentesting/873-pentesting-rsync.html) covers some of the ways Rsync can be abused. Rsync typically use port `873`.
#### Probing for Accessible Shares
```bash
hcx05@htb[/htb]$ nc -nv [Target host ip] 873

(UNKNOWN) [127.0.0.1] 873 (rsync) open
@RSYNCD: 31.0
@RSYNCD: 31.0
#list
dev             Dev Tools
@RSYNCD: EXIT
```
#### Enumerating an Open Share
```bash
hcx05@htb[/htb]$ rsync -av --list-only rsync://[Target host ip]/dev
#-a: Archive mode, whiching all the properity of the archives, including recursive, permissions, modification times, owner/group and symlinks.

receiving incremental file list
drwxr-xr-x             48 2022/09/19 09:43:10 .
-rw-r--r--              0 2022/09/19 09:34:50 build.sh
-rw-r--r--              0 2022/09/19 09:36:02 secrets.yaml
drwx------             54 2022/09/19 09:43:10 .ssh

sent 25 bytes  received 221 bytes  492.00 bytes/sec
total size is 0  speedup is 0.00
```
f Rsync is configured to use SSH to transfer files, we could modify our commands to include the `-e ssh` flag, or `-e "ssh -p2222"` if a non-standard port is in use for SSH. This [guide](https://phoenixnap.com/kb/how-to-rsync-over-ssh) is helpful for understanding the syntax for using Rsync over SSH.




































