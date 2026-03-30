---

```bash
uname -a 
	#Detial information about the kernal used by the system
 
/proc/version 
	#The proc filesystem (procfs), may give you information on the kernel version and additional data such as whether a compiler (e.g. GCC) is installed.

/etc/os-release
	#Check the actual version of the OS

/etc/issue
	#Usually contains some information about the operating system but can easily be customized or changed.

ps -A
	#View all running processes
ps axjf
	#See process tree
ps aux
	#The `aux` option will show processes for all users (a), display the user that launched the process (u), and show processes that are not attached to a terminal (x).

env
	#Show environmental variables.

ss -a
	#Shows all listening ports and established connections.
ss -atu
	#List TCP and UDP protocals.
ss -tunlp
	#Shows all listening TCP/UDP ports, -n means don't resolve names.



```
