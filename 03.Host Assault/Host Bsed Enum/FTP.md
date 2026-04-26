
---
Active FTP: Communication port 21, transmission port 20.

Passive FTP: Communication port 21, transmission port > 1024.

### TFTP
FTP use TCP, while TFTP use UDP, making it an unreliable protocol and causinig it to use UDP-assisted application layer recovery.

TFTP does not require the user's authentication.Because of the lack of security, TFTP, unlike FTP, may only be used in local and protected networks.

Unlike the FTP client, TFTP does not directory listing functionality.

#### Commanmd of TFTP
- `connect`: Sets the remote host, and optionally the port, for file transfers.
- `get`: Transfers a file or set of files from the remote host to the local host.
- `put`: Transfers a file or set of files from the local host onto the remote host.
- `quit`: Exits tftp.
- `status`: Shows the current status of tftp, including the current transfer mode (ascii or binary), connection status, time-out value, and so on.
- `verbose`: Display additional information.

### vsFTPd
The default configuration of vsFTPd is `/etc/vsftp.conf`, this is the most used FTP serverson Linux-based distribution.

In addition, there is a file called `/etc/ftpusers` that we also need to pay attention to, as this file is used to deny certain users access to the FTP service.

#### Dangerous Settings
- `anonymous_enable=YES`: Allowing anonymous login?
- `anon_upload_enable=YES`: Allowing anonymous to upload files?
- `anon_mkdir_write_enable=YES`: Allowing anonymous to create new directories?
- `no_anon_password=YES`: Do not ask anonymous for password?
- `anon_root=/home/username/ftp`: Directory for anonymous.
- `write_enable=YES`: Allow the usage of FTP commands: STOR, DELE, RNFR, RNTO, MKD, RMD, APPE, and SITE?

Get a first overiew as an anonymous user:
```bash
ftp> status 

Connected to 10.129.14.136. 
No proxy connection. 
Connecting using address family: any. 
Mode: stream; Type: binary; Form: non-print; Structure: file Verbose: on; Bell: off; Prompting: on; Globbing: on 
Store unique: off; Receive unique: off 
Case: off; CR stripping: on 
Quote control characters: on 
Ntrans: off 
Nmap: off Hash mark printing: off; Use of PORT cmds: on 
Tick counter printing: off
```

#### vsFTPd Detailed Output
```bash
ftp> debug

Debugging on (debug=1).


ftp> trace

Packet tracing on.


ftp> ls

---> PORT 10,10,14,4,188,195
#Port 48128: 188*256+195
200 PORT command successful. Consider using PASV.
---> LIST
150 Here comes the directory listing.
-rw-rw-r-- 1 1002 1002 8138592 Sep 14 16:54 Calender.pptx
drwxrwxr-x 2 1002 1002 4096 Sep 14 17:03 Clients
drwxrwxr-x 2 1002 1002 4096 Sep 14 16:50 Documents
drwxrwxr-x 2 1002 1002 4096 Sep 14 16:50 Employees
-rw-rw-r-- 1 1002 1002 41 Sep 14 16:45 Important Notes.txt
226 Directory send OK
```

#### Hiding IDs - YES
All user and group information in directory listings will be displayed as "ftp".
```bash
ftp> ls

---> TYPE A
200 Switching to ASCII mode.
ftp: setsockopt (ignored): Permission denied
---> PORT 10,10,14,4,223,101
200 PORT command successful. Consider using PASV.
---> LIST
150 Here comes the directory listing.
-rw-rw-r-- 1 ftp ftp 8138592 Sep 14 16:54 Calender.pptx
drwxrwxr-x 2 ftp ftp 4096 Sep 14 17:03 Clients
drwxrwxr-x 2 ftp ftp 4096 Sep 14 16:50 Documents
drwxrwxr-x 2 ftp ftp 4096 Sep 14 16:50 Employees
-rw-rw-r-- 1 ftp ftp 41 Sep 14 16:45 Important Notes.txt
-rw------- 1 ftp ftp 0 Sep 15 14:57 testupload.txt
226 Directory send OK.
```

#### ls_recurse_enable - YES
```bash
ftp> ls -R

---> PORT 10,10,14,4,222,149
200 PORT command successful. Consider using PASV.
---> LIST -R
150 Here comes the directory listing.
.:
-rw-rw-r-- 1 ftp ftp 8138592 Sep 14 16:54 Calender.pptx
drwxrwxr-x 2 ftp ftp 4096 Sep 14 17:03 Clients
drwxrwxr-x 2 ftp ftp 4096 Sep 14 16:50 Documents
drwxrwxr-x 2 ftp ftp 4096 Sep 14 16:50 Employees
-rw-rw-r-- 1 ftp ftp 41 Sep 14 16:45 Important Notes.txt
-rw------- 1 ftp ftp 0 Sep 15 14:57 testupload.txt

./Clients:
drwx------ 2 ftp ftp 4096 Sep 16 18:04 HackTheBox
drwxrwxrwx 2 ftp ftp 4096 Sep 16 18:00 Inlanefreight

<SNIP>
```

#### Data Exfiltration
Download All Available Files
```bash
hcx05@htb[/htb]$ wget -m --no-passive ftp://anonymous:anonymous@10.129.14.136
#-m: Mirror mode, executing recursive download.
#--no-passive: More efficient.
#Format: ftp://user:password@IP
```

#### Upload a File
```bash
hcx05@htb[/htb]$ touch testupload.txt

ftp> put testupload.txt
```

### Footprinting the Service
#### Service Interaction
 If necessary, we can, of course, use other applications such as `netcat` or `telnet` to interact with the FTP server.
```bash
nc -nv 10.129.14.136 21
```

```bash
telnet 10.129.14.136 21
```

If the FTP server runs with TLS/SSL encryption, we use the client `openssl` and communicate with the FTP server.
```basah
hcx05@htb[/htb]$ openssl s_client -connect 10.129.14.136:21 -starttls ftp
```
The SSL certificate allows us to recognize the `hostname`, for example, and in most cases also an `email address` for the organization or company. In addition, if the company has several locations worldwide, certificates can also be created for specific locations, which can also be identified using the SSL certificate.