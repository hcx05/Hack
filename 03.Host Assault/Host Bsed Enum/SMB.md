
---
SMB: Server Message Block

Is a client-server protocol that regulates access to files and entire directories and other network resources such as printers, routers, or interfaces released for the network. Which is in the application layer.

An SMB server's access rights are defined by `Access Control Lists` (`ACL`). The ACLs are defined based on the shares and therefore do not correspond to the rights assigned locally on the server.

### Samba
A solution that enables the use of SMB in Linux and Unix distributions and thus cross-platform communication via SMB.

Before Windows 2000, SMB protocol must run on NetBIOS, that's why the old system use TCP 139 port&　UDP 137/138 ports.

#### Default Configuration
```bash
cat /etc/samba/smb.conf | grep -v "#\|\;"
```

`[sharename]`: The name of the network share.

`workgroup = WORKGROUP/DOMAIN`: Workgroup that will appear when clients query.

`path = /path/here/`: The directory to which user is to be given access.

`server string = STRING`: The string that will show up when a connection is initiated.

`unix password sync = yes`: Synchronize the UNIX password with the SMB password?

`usershare allow guests = yes`: Allow non-authenticated users to access defined share?

`map to guest = bad user`: What to do when a user login request doesn't match a valid UNIX user?

`read only = yes`: Allow users to read files only?

#### Dangerous Settings
`browseable = yes`: Allow listing available shares in the current share?

`read only = no`: Forbid the creation and modification of files?

`writable = yes`: Allow users to create and modify files?

`guest ok = yes`: Allow connecting to the service without using a password?

`create mask = 0700`: What permissions need to be set for newly created files?

`directory mask = 0777`: What permissions must be assigned to the newly created directories?

`logon script = script.sh`: What script needs to be executed on the user's login?

`magic script = script.sh`: Which script should be executed when the script gets closed?

`magic output = script.out`: Where the output of the magic script needs to be stored?

#### Restart Samba
If we've sdjkisted the `etc/samba/smg.conf`, it has to be restart.
```bash
sudo systemctl restart smbd
```

#### SMBclient - Connecting to the Share
```bash
hcx05@htb[/htb]$ smbclient -N -L //10.129.14.128 

	Sharename Type Comment 
	--------- ---- ------- 
	print$    Disk Printer Drivers 
	home      Disk INFREIGHT Samba 
	dev       Disk DEVenv 
	notes     Disk CheckIT 
	IPC$      IPC  IPC Service (DEVSM) 
SMB1 disabled -- no workgroup available
```

```bash
hcx05@htb[/htb]$ smbclient //10.129.14.128/notes 

Enter WORKGROUP\<username>'s password: 
Anonymous login successful 
Try "help" to get a list of possible commands.

smb: \> ls

	.                           D    0  Wed Sep 22 18:17:51 2021
	..                          D    0  Wed Sep 22 12:03:59 2021
	prep-prod.txt               N    71 Sun Sep 19 15:45:21 2021
	
		30313412 blocks of size 1024. 16480084 blocks available
```
Smbclient also allows us to execute local system commands using an exclamation mark at the beginning (`!<cmd>`) without interrupting the connection.

Commands:
`ls`, `get`, `put`

#### Status
```basah
root@samba:~# smbstatus
```

### RPCclient
```bash
hcx05@htb[/htb]$ rpcclient -U "" -N 10.129.14.128 

rpcclient $>
```

Commands:
`srvinfo`: Server information.

`enumdomains`: Enumerate all domains that are deployed in the network.

`querydominfo`: Provides domain, server, and user information of deployed domains.

`netshareenumall`: Enumerates all available shares.

`netsharegetinfo <share>`: Provides information about a specific share.

`enumdomusers`: Enumerates all domain users.

`queryuser <RID>`: Provides information about a specific user.

`querygroup <RID>`: Provides information about a specific group.

#### Brute Forcing User RIDs
```bash
$ for i in $(seq 500 1100);do rpcclient -N -U "" 10.129.14.128 -c "queryuser 0x$(printf '%x\n' $i)" | grep "User Name\|user_rid\|group_rid" && echo "";done
#RID 500 is Administrator in default.
#RID 501 is Guest in default.
#RID 1000+: Usually users that created manually.
```

An alternative solution from [Impacket](https://github.com/SecureAuthCorp/impacket) is [samrdump.py](https://github.com/SecureAuthCorp/impacket/blob/master/examples/samrdump.py)
```bash
samrdump.py 10.129.14.128
```

### Other enum tools
#### SMBmap
[SMBMap](https://github.com/ShawnDEvans/smbmap)
```bash
smbmap -H 10.129.14.128
```

#### CrackMapExec
```bash
crackmapexec smb 10.129.14.128 --shares -u '' -p ''
```

#### Enum4Linux-ng
Istallation
```bash
git clone https://github.com/cddmp/enum4linux-ng.git

cd enum4linux-ng
```

Then install `requirements.txt`

```bash
./enum4linux-ng.py 10.129.14.128 -A
```