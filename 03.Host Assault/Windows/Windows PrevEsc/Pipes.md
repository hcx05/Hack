
---
#### **1. Named Pipes**

- **Definition:** A named, virtual file stored in **RAM** used for communication between unrelated processes (even across a network).
    
- **Path Format:** `\\.\pipe\PipeName`
    
- **Key Characteristics:**
    
    - **Persistence:** Exists until the server process closes it or the system reboots.
        
    - **Accessibility:** Can be accessed by any process with sufficient permissions (Client-Server model).
        
    - **Network:** Supports communication over **SMB** (Port 445).
        
- **Red Team Note:** Often used for **C2 Beacon communication** and **Privilege Escalation** (e.g., _PrintSpoofer_ via `SeImpersonatePrivilege`).
    

---

#### **2. Anonymous Pipes**

- **Definition:** An unnamed, temporary memory channel used for communication between a **parent process** and its **child process**.
    
- **Path Format:** None (No addressable name).
    
- **Key Characteristics:**
    
    - **Persistence:** Destroyed immediately when the process handles are closed.
        
    - **Accessibility:** Limited to local parent-child inheritance; outsiders cannot "join" the pipe.
        
    - **Direction:** Strictly **Half-duplex** (One-way only).
        
- **Red Team Note:** Commonly seen in standard command redirection, like the `|` (pipe) operator in `cmd.exe` (e.g., `dir | findstr "txt"`).

|**Feature**|**Named Pipe**|**Anonymous Pipe**|
|---|---|---|
|**Identification**|Unique Name (`\\.\pipe\...`)|Handle-based (No Name)|
|**Scope**|Local & Network|Local Only (Parent/Child)|
|**Communication**|Duplex (Two-way)|Half-duplex (One-way)|
|**Visibility**|Visible via `pipelist.exe`|Invisible to external tools|
#### Listing Named Pipes with Pipelist
 [PipeList](https://docs.microsoft.com/en-us/sysinternals/downloads/pipelist)
 ```bash
 C:\htb> pipelist.exe /accepteula
 #/accepteula: Auto accept 'End User Lience Agreement'
 ```
 Using `gci`(`Get-ChildItem) in PowerShell
 ```powershell
 PS C:\htb> gci \\.\pipe\
 ```
#### Reviewing LSASS Named Pipe Permissions
[Accesschk](https://docs.microsoft.com/en-us/sysinternals/downloads/accesschk)
```cmd
C:\htb> accesschk.exe /accepteula \\.\Pipe\lsass -v
```


















































