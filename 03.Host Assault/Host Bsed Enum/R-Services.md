
---
`R-services` span across the ports `512`, `513`, and `514` and are only accessible through a suite of programs known as `r-commands`.

The [R-commands](https://en.wikipedia.org/wiki/Berkeley_r-commands) suite:
- rcp (`remote copy`)
- rexec (`remote execution`)
- rlogin (`remote login`)
- rsh (`remote shell`)
- rstat
- ruptime
- rwho (`remote who`)

|**Command**|**Service Daemon**|**Port**|**Transport Protocol**|**Description**|
|---|---|---|---|---|
|`rcp`|`rshd`|514|TCP|Copy a file or directory bidirectionally from the local system to the remote system (or vice versa) or from one remote system to another. It works like the `cp` command on Linux but provides `no warning to the user for overwriting existing files on a system`.|
|`rsh`|`rshd`|514|TCP|Opens a shell on a remote machine without a login procedure. Relies upon the trusted entries in the `/etc/hosts.equiv` and `.rhosts` files for validation.|
|`rexec`|`rexecd`|512|TCP|Enables a user to run shell commands on a remote machine. Requires authentication through the use of a `username` and `password` through an unencrypted network socket. Authentication is overridden by the trusted entries in the `/etc/hosts.equiv` and `.rhosts` files.|
|`rlogin`|`rlogind`|513|TCP|Enables a user to log in to a remote host over the network. It works similarly to `telnet` but can only connect to Unix-like hosts. Authentication is overridden by the trusted entries in the `/etc/hosts.equiv` and `.rhosts` files.|

#### Access Control & Trusted Relationships
By default, these services utilize [Pluggable Authentication Modules (PAM)](https://web.archive.org/web/20241102161436/https://debathena.mit.edu/trac/wiki/PAM) for user authentication onto a remote system; however, they also bypass this authentication through the use of the `/etc/hosts.equiv` and `.rhosts` files on the system. 

**Note:** The `hosts.equiv` file is recognized as the global configuration regarding all users on a system, whereas 
`.rhosts` provides a per-user configuration.
#### /etc/hosts.equiv
```bash
hcx05@htb[/htb]$ cat /etc/hosts.equiv

# <hostname> <local username>
pwnbox cry0l1t3
```
#### Sample .rhosts File
```bash
hcx05@htb[/htb]$ cat .rhosts

htb-student     10.0.17.5
+               10.0.17.10
+               +
```
`+` modifier can be used within these files as a wildcard to specify anything.
#### Logging in Using Rlogin
```bash
hcx05@htb[/htb]$ rlogin 10.0.17.2 -l htb-student

Last login: Fri Dec  2 16:11:21 from localhost

[htb-student@localhost ~]$
```
#### Listing Authenticated Users Using Rwho
We can also abuse the `rwho` command to list all interactive sessions on the local network by sending requests to the UDP port 513.

Using broadcast.
```bash
hcx05@htb[/htb]$ rwho

root     web01:pts/0 Dec  2 21:34
htb-student     workstn01:tty1  Dec  2 19:57  2:25
```
#### Listing Authenticated Users Using Rusers
Using RPC to query the target host.
```bash
hcx05@htb[/htb]$ rusers -al 10.0.17.5
#-a: list all users.
#-l: long format.

htb-student     10.0.17.5:console          Dec 2 19:57     2:25
#2:25 is the idle time of the user.
```



































