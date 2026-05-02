
---
### Check Windows Defender Status
```powershell
PS C:\htb> Get-MpComputerStatus
```

#### List AppLocker Rules
```powershell
PS C:\htb> Get-AppLockerPolicy -Effective | select -ExpandProperty RuleCollections
```

#### Test AppLocker Policy
```powershell
PS C:\htb> Get-AppLockerPolicy -Local | Test-AppLockerPolicy -path C:\Windows\System32\cmd.exe -User Everyone
```

### System Information
#### Running Processes
```cmd
C:\htb> tasklist /svc
```
##### Standard Windows processes
1. **smss.exe (Session Manager Subsystem)**

- **What it is:** The first user-mode process to start in Windows.
    
- **Job:** It creates environment variables and starts other key processes like `winlogon.exe` and `csrss.exe`.
    
- **Red Team Note:** There should only be **one** master `smss.exe` running from `C:\Windows\System32`. If you see it elsewhere, it’s highly suspicious.
    

2. **csrss.exe (Client Server Runtime Subsystem)**

- **What it is:** The graphical instruction set of Windows.
    
- **Job:** Handles Win32 console windows and creating/deleting threads.
    
- **Red Team Note:** This process always runs as **SYSTEM**. It is a common target for process hollowing or injection.
    

3. **winlogon.exe (Windows Logon Process)**

- **What it is:** The process responsible for handling user logons and logoffs.
    
- **Job:** It handles the **SAS** (Secure Attention Sequence), which is the `Ctrl+Alt+Delete` screen.
    
- **Red Team Note:** If you gain enough privileges, you might intercept credentials here or use it to trigger "Sticky Keys" backdoors (`sethc.exe`).
    

4. **lsass.exe (Local Security Authority Subsystem Service)**

- **What it is:** The "Vault" of Windows security.
    
- **Job:** It enforces security policies, verifies passwords, and creates access tokens.
    
- **Red Team Note:** **CRITICAL TARGET.** This process stores credentials in memory. Red Teamers use tools like `Mimikatz` to "dump" `lsass.exe` to steal hashes or cleartext passwords.
    

5. **svchost.exe (Service Host)**

-**What it is:** A generic host process for services that run from DLLs.
    
- **Job:** Instead of having 50 separate `.exe` files for small tasks (like Windows Update or DNS), Windows groups them under several `svchost.exe` instances.
    
- **Red Team Note:** Since there are usually many `svchost.exe` running, attackers love to hide malicious services or code inside them to "blend in."
#### Display All Environment Variables
```cmd
C:\htb> set

ALLUSERSPROFILE=C:\ProgramData APPDATA=C:\Users\Administrator\AppData\Roaming CommonProgramFiles=C:\Program Files\Common Files CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files CommonProgramW6432=C:\Program Files\Common Files 
COMPUTERNAME=WINLPE-SRV01 
ComSpec=C:\Windows\system32\cmd.exe

<SNIP>
```
#### View Detailed Configuration Information
```cmd
C:\htb> systeminfo
```
#### Patches and Updates

If `systeminfo` doesn't display hotfixes, they may be queriable with WMI using the WMI-Command binary with QFE (Quick Fix Engineering) to display patches.
```cmd
C:\htb> wmic qfe
```
Or
```cmd
C:\htb> wmic qfe get Caption,Description,HotFixID,InstalledOn
```
Using Powershell
```powershell
PS C:\htb> Get-HotFix | ft -AutoSize
#ft: Format-Table
```
Permutation by date
```powershell
Get-HotFix | Sort-Object InstalledOn -Descending | ft -AutoSize
```
#### Installed Programs
```cmd
C:\htb> wmic product get name,version
```
Do this on Powershell
```powershell
PS C:\htb> Get-CimInstance -ClassName Win32_Product | select Name, Version
```
#### Display Running Processes
```powershell
PS C:\htb> netstat -ano
#-a: All port
#-o: Display PID
```
#### Checking the service
```cmd
C:\htb> tasklist /svc /fi "pid eq [PID]"
```
### User & Group Information
#### Checking Sessions Type
```cmd
C:\htb> qwinsta
```
#### Logged-In Users
```cmd
C:\htb> query user
```
#### Current User
```cmd
C:\htb> echo %USERNAME%
```
#### Current User Privileges
```cmd
C:\htb> whoami /priv
```
#### Current User Group Information
```cmd
C:\htb> whoami /groups
```
#### Get All Users
```cmd
C:\htb> net user
```
#### Get All Groups
```cmd
C:\htb> net localgroup
```
#### Details About a Group
```cmd
C:\htb> net localgroup <group's name>
```
#### Get Password Policy & Other Account Information
```cmd
C:\htb> net accounts
```

### Cheat Sheets
[WinEmun](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Windows%20-%20Privilege%20Escalation.md)





























