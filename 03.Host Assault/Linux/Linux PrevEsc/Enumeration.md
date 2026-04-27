---

```bash
uname -a 
	#Detial information about the kernal used by the system

hcx05@htb[/htb]$ cat /etc/passwd | cut -f1 -d:

root
daemon
bin
sys

...SNIP...

mrb3n
lxd
bjones
administrator.ilfreight
backupsvc
cliff.moore
logger
shared
stacey.jenkins
htb-student
	#-d : Delimiter, cut when seeing :
	#-f1 : Discard after field 1.

grep "sh$" /etc/passwd

/proc/version 
	#The proc filesystem (procfs), may give you information on the kernel version and additional data such as whether a compiler (e.g. GCC) is installed.

/etc/os-release
	#Check the actual version of the OS

/etc/issue
	#Usually contains some information about the operating system but can easily be customized or changed.

lscpu

ps -A
	#View all running processes
ps axjf
	#See process tree
ps aux
	#The `aux` option will show processes for all users (a), display the user that launched the process (u), and show processes that are not attached to a terminal (x).

env
	#Show environmental variables, we may get lucky and find something sensitive in there such as a password.

ss -a
	#Shows all listening ports and established connections.
ss -atu
	#List TCP and UDP protocals.
ss -tunlp
	#Shows all listening TCP/UDP ports, -n means don't resolve names.

route

```

Unmounted File Systems and Additional Drives : If you discover and can mount an additional drive or unmounted file system, you may find sensitive files, passwords, or backups that can be leveraged to escalate privileges.

```bash

lsblk

NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sda      8:0    0   30G  0 disk 
├─sda1   8:1    0   29G  0 part /
├─sda2   8:2    0    1K  0 part 
└─sda5   8:5    0  975M  0 part [SWAP]
sr0     11:0    1  848M  0 rom

	#sda1 : Mounted place is /, SSD or HDD.
	#sda5 : Mounted place is [SWAP], virtual memory.
	#sda2 : Empty, means that the partition exist but haven't been read.
	#sr0 : It's often a sr0.

sudo mount /dev/sda2 /mnt
	#Try to mount sda2 to /mnt, if we don't have privilege, go to /etc/fstab and find somethig interesting.

```

```

lpstat
	#Find information about any printers attached to the system. If there are active or queued print jobs can we gain access to some sort of sensitive information.

```

Find writeable Dir/FIles

```bash

hcx05@htb[/htb]$ find / -path /proc -prune -o -type d -perm -o+w 2>/dev/null

hcx05@htb[/htb]$ find / -path /proc -prune -o -type f -perm -o+w 2>/dev/null

```

```bash

hcx05@htb[/htb]$ cat /etc/shells

# /etc/shells: valid login shells
/bin/sh
/bin/bash
/usr/bin/bash
/bin/rbash
/usr/bin/rbash #Restricted bash shell.
/bin/dash #/bin/sh point to here default in ubuntu.
/usr/bin/dash
/usr/bin/tmux
/usr/bin/screen

screen -ls

tmux ls

```

```bash

hcx05@htb[/htb]$ cat /etc/group

root:x:0:
daemon:x:1:
bin:x:2:
sys:x:3:
adm:x:4:syslog,htb-student
tty:x:5:syslog
disk:x:6:
lp:x:7:
mail:x:8:
news:x:9:
uucp:x:10:
man:x:12:
proxy:x:13:
kmem:x:15:
dialout:x:20:
fax:x:21:
voice:x:22:
cdrom:x:24:htb-student
floppy:x:25:
tape:x:26:
sudo:x:27:mrb3n,htb-student
audio:x:29:pulse
dip:x:30:htb-student
www-data:x:33:
...SNIP...

hcx05@htb[/htb]$ getent group sudo

sudo:x:27:mrb3n

```

In Linux, there are many different places where such files can be stored, including mounted file systems. A mounted file system is a file system that is attached to a particular directory on the system and accessed through that directory. Many file systems, such as ext4, NTFS, and FAT32, can be mounted. Each type of file system has its own benefits and drawbacks. For example, some file systems can only be read by the operating system, while others can be read and written by the user. File systems that can be read and written to by the user are called read/write file systems. Mounting a file system allows the user to access the files and folders stored on that file system. In order to mount a file system, the user must have root privileges. Once a file system is mounted, it can be unmounted by the user with root privileges. We may have access to such file systems and may find sensitive information, documentation, or applications there.

Mounted File Systems

```bash

df -h

```

Unmounted File Systems

```bash

cat /etc/fstab | grep -v "#" | column -t

```

All Hidden Files/Dir

```bash

find / -type f -name ".*" -exec ls -l {} \; 2>/dev/null | grep ownername

hcx05@htb[/htb]$ find / -type d -name ".*" -ls 2>/dev/null

```

Temporary files :

```bash

/tmp
	# The data is automatically deleted after ten days
/var/tmp
	# Retention time is much longer than that of the /tmp directory.
	# By default, all files and data stored in /var/tmp are retained for up to 30 days.

ls -l /tmp /var/tmp /dev/shm
	# /dev/shm is a directory that exit in RAM.

```

Find history files :

```bash

hcx05@htb[/htb]$ find / -type f \( -name *_hist -o -name *_history \) -exec ls -l {} \; 2>/dev/null

-rw------- 1 htb-student htb-student 387 Nov 27 14:02 /home/htb-student/.bash_history

```

The proc filesystem (proc / procfs) is a particular filesystem in Linux that contains information about system processes, hardware, and other system information. It is the primary way to access process information and can be used to view and modify kernel settings. It is virtual and does not exist as a real filesystem but is dynamically generated by the kernel.

`ps` command is eccentially reading /proc, but it may fliter something.

```bash

hcx05@htb[/htb]$ find /proc -name cmdline -exec cat {} \; 2>/dev/null | tr " " "\n"

...SNIP...
startups/usr/lib/packagekit/packagekitd/usr/lib/packagekit/packagekitd/usr/lib/packagekit/packagekitd/usr/lib/packagekit/packagekitdroot@10.129.14.200sshroot@10.129.14.200sshd:
htb-student
[priv]sshd:
htb-student
[priv]/usr/bin/ssh-agent-D-a/run/user/1000/keyring/.ssh/usr/bin/ssh-agent-D-a/run/user/1000/keyring/.sshsshd:
htb-student@pts/2sshd:

```

If it is a slightly older Linux system, the likelihood increases that we can find installed packages that may already have at least one vulnerability. However, current versions of Linux distributions can also have older packages or software installed that may have such vulnerabilities. 

```bash

hcx05@htb[/htb]$ apt list --installed | tr "/" " " | cut -d" " -f1,3 | sed 's/[0-9]://g' | tee -a installed_pkgs.list

#sed 's/[0-9]://g' : Is a regular expression, use to delete Epoch codes before version number.

Listing...                                                 
accountsservice-ubuntu-schemas 0.0.7+17.10.20170922-0ubuntu1                                                          
accountsservice 0.6.55-0ubuntu12~20.04.5                   
acl 2.2.53-6                                               
acpi-support 0.143                                         
acpid 2.0.32-1ubuntu1                                      
adduser 3.118ubuntu2                                       
adwaita-icon-theme 3.36.1-2ubuntu0.20.04.2                 
alsa-base 1.0.25+dfsg-0ubuntu5                             
alsa-topology-conf 1.2.2-1                                                                                            
alsa-ucm-conf 1.2.2-1ubuntu0.13                            
alsa-utils 1.2.2-1ubuntu2.1                                                                                           
amd64-microcode 3.20191218.1ubuntu1
anacron 2.3-29
apg 2.2.3.dfsg.1-5
app-install-data-partner 19.04
apparmor 2.13.3-7ubuntu5.1
apport-gtk 2.20.11-0ubuntu27.24
apport-symptoms 0.23
apport 2.20.11-0ubuntu27.24
appstream 0.12.10-2
apt-config-icons-hidpi 0.12.10-2
apt-config-icons 0.12.10-2
apt-utils 2.0.9
...SNIP...

```

It's also a good idea to check if the sudo version installed on the system is vulnerable to any legacy or recent exploits.

```bash

sudo -V

```

Occasionally it can also happen that no direct packages are installed on the system but compiled programs in the form of binaries. These do not require installation and can be executed directly by the system itself.

```bahs

hcx05@htb[/htb]$ ls -l /bin /usr/bin/ /usr/sbin/

```

Use GTFObins

```bahs

for i in $(curl -s https://gtfobins.org/api.json | jq -r '.executables | keys[]'); do if grep -q "$i" installed_pkgs.list; then echo "Check for GTFO: $i";fi; done

# -s : silent mode, not showing download progress
# jq : prases the JSON format

```

Trace System Calls

```bash

	hcx05@htb[/htb]$ strace ping -c1 {random ip}

```

Script

```bash

hcx05@htb[/htb]$ find / -type f -name "*.sh" 2>/dev/null | grep -v "src\|snap\|share"

/home/htb-student/automation.sh
/etc/wpa_supplicant/action_wpa.sh
/etc/wpa_supplicant/ifupdown.sh
/etc/wpa_supplicant/functions.sh
/etc/init.d/keyboard-setup.sh
/etc/init.d/console-setup.sh
/etc/init.d/hwclock.sh
...SNIP...

```

```bash

find / ! -path "*/proc/*" -iname "*config*" -type f -not -path "*/usr/*" 2>/dev/null
	#"" is essential to let find to prase *, without "", bash will phrase it on the other hand.
	#-not -path "*/usr*" is useful to exclude system default configuration file.

```





