
---
CME heavily uses the [Impacket](https://github.com/SecureAuthCorp/impacket) library to work with network protocols and perform a variety of post-exploitation techniques.

On June 2023, [mpgn](https://twitter.com/mpgn_x64), the lead developer of CrackMapExec, has created a new repository containing [CrackMapExec version 6](https://github.com/mpgn/CrackMapExec), the latest version of CrackMapExec, but it was later removed.

Some of the developers who contributed to the tool decided to create a fork to continue the project. The project was renamed to `NetExec` and is at [https://github.com/Pennyw0rth/NetExec](https://github.com/Pennyw0rth/NetExec).
### Linux Installation
CrackMapExec developers recommend using [Poetry](https://python-poetry.org/docs/) for dependency and package management. [Poetry](https://python-poetry.org/docs/) is a tool for dependency management and packaging in Python. It allows you to declare the libraries your project depends on, and it will manage (install/update) them for you.
#### Installing Poetry
```bash
curl -SSL https://install.python-poetry.org | python3 -
```
#### Installing CrackMapExec Required Libraries
```bash
sudo apt-get update
sudo apt-get install -y libssl-dev libkrb5-dev libffi-dev python-dev build-essential
```
#### Installing Rust
CrackMapExec requires a library for the RDP protocol that uses Rust. We will use the following command to install Rust.
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs/ | sh
```
Next, we should close the terminal; otherwise, we will get an error when installing the RDP library `aardwolf`. Once the terminal is closed, we open a new one and proceed with the installation.
#### Installing CrackMapExec with Poetry
```bash
git clone https://github.com/Porchetta-Industries/CrackMapExec
cd CrackMapExec
portry install
```
#### Using Poetry Shell
If we want to avoid using `poetry run` before `crackmapexec`, we can execute `poetry shell` within the installation directory to activate the Poetry virtual environment.
```bash
hcx05@htb[/htb]$ cd CrackMapExec
hcx05@htb[/htb]$ poetry shell

Spawning shell within /home/htb-XXXXXXX/.cache/pypoetry/virtualenvs/crackmapexec-4YDbTJlJ-py3.9
```
```bash
(crackmapexec-py3.9) hcx05@htb[/htb]$ crackmapexec --help 
usage: crackmapexec [-h] [-t THREADS] [--timeout TIMEOUT] [--jitter INTERVAL] [--darrell] [--verbose] {ftp,ldap,mssql,rdp,smb,ssh,winrm} ...

<SNIP>
```
**Note:** We can identify that we are in the Poetry shell if we see `(crackmapexec-py3.X)`at the beginning of our terminal. To deactivate the virtual environment and exit this new shell, type `exit`. To deactivate the virtual environment without leaving the shell, use `deactivate`.
### Installation for Docker
CrackMapExec is Docker-friendly. We can use the `Dockerfile` in the [GitHub repository](https://github.com/Porchetta-Industries/CrackMapExec) to build it from the source.
#### Installing Docker using the GitHub repository
```bash
sudo apt install docker.io
git clone https://github.com/Porchetta-Industries/CrackMapExec -q
cd CrackMapExec
sudo docker build -t crackmapexec .
```
```bash
sudo docker run -it --entrypoint=/bin/bash --name crackmapexec -v ~/.cme:/root/.cme crackmapexec
```
```bash
root@d46e1e7925dc:/usr/src/crackmapexec/cme\# crackmapexec

[*] Creating default workspace
[*] Initializing LDAP protocol database
[*] Initializing MSSQL protocol database
[*] Initializing RDP protocol database
[*] Initializing SMB protocol database
[*] Initializing SSH protocol database
[*] Initializing WINRM protocol database
[*] Copying default configuration file
[*] Generating SSL certificate
usage: crackmapexec [-h] [-t THREADS] [--timeout TIMEOUT] [--jitter INTERVAL] [--darrell] [--verbose] {ftp,ssh,winrm,mssql,rdp,LDAP,smb} ...

      ______ .______           ___        ______  __  ___ .___  ___.      ___      .______    _______ ___   ___  _______   ______
     /      ||   _  \         /   \      /      ||  |/  / |   /   |     /   \     |   _  \  |   ____|\  \ /  / |   ____| /      |
    |  ,----'|  |_)  |       /  ^  \    |  ,----'|  '  /  |  \  /  |    /  ^  \    |  |_)  | |  |__    \  V  /  |  |__   |  ,----'
    |  |     |      /       /  /_\  \   |  |     |    <   |  |/|  |   /  /_\  \   |   ___/  |   __|    >   <   |   __|  |  |
    |  `----.|  |\  \----. /  _____  \  |  `----.|  .  \  |  |  |  |  /  _____  \  |  |      |  |____  /  .  \  |  |____ |  `----.
     \______|| _| `._____|/__/     \__\  \______||__|\__\ |__|  |__| /__/     \__\ | _|      |_______|/__/ \__\ |_______| \______|

                                                A swiss army knife for pentesting networks
                                    Forged by @byt3bl33d3r and @mpgn_x64 using the powah of dank memes

                                           Exclusive release for Porchetta Industries users
                                                       https://porchetta.industries/

                                                   Version : 5.4.0
                                                   Codename: Indestructible G0thm0g

<SNIP>
```
#### Restart Container
After exiting the container, we can restart it using the following command:
```bash
hcx05@htb[/htb]$ sudo docker start crackmapexec
hcx05@htb[/htb]$ sudo docker exec -it crackmapexec bash
root@dbbda0e6bf72:/usr/src/crackmapexec#
```
### Binaries
```powershell
pip install pipx
python -m pipx ensurepath
python -m pipx install git+https://github.com/Pennyw0rth/NetExec
```
#### Setting Long Path Registry Key
```cmd
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem" /v LongPathsEnabled /t REG_DWORD /d 1 /f
```
**Note:** CrackMapExec save its logs and database at **~/.cme** directory.
### Supported Protocols
| **Protocol** | **Default Port** |
| ------------ | ---------------- |
| SMB          | 445              |
| WINRM        | 5985/5986        |
| MSSQL        | 1433             |
| LDAP         | 389              |
| SSH          | 22               |
| RDP          | 3389             |
| FTP          | 21               |
### Export Function
```bash
crackmapexec smb 10.10.10.1 [protocol options] --export $(pwd)/export.txt
```
### Protocol Modules
`crackmapexec <protocol> -L` to view available modules for the specified protocol.
