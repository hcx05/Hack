
---
[Sshuttle](https://github.com/sshuttle/sshuttle) is another tool written in Python which removes the need to configure proxychains. However, this tool only works for pivoting over SSH and does not provide other options for pivoting over TOR or HTTPS proxy servers. `Sshuttle` can be extremely useful for automating the execution of iptables and adding pivot rules for the remote host.
```bash
sudo apt-get install sshuttle
```
To use sshuttle, we specify the option `-r` to connect to the remote machine with a username and password.
#### Running sshuttle
```bash
sudo sshuttle -r ubuntu@10.129.202.64 172.16.5.0/23 -v
```
