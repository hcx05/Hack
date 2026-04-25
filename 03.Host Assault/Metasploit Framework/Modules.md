
---
### Syntax
```bash
<No.> <type>/<os>/<service>/<name>
```

```bash
794 exploit/windows/ftp/scriptftp_list
```

### Type
`Auxiliary`: Scanning, fuzzing, sniffing, and admin capabilities. Offer extra assistance and functionality.

`ENcoders`: Ensure that payloads are intact to their destination.

`Exploits`: Defined as modules that exploit a vulnerability that will allow for the payload delivery.

`NOPs`: (No Operation code) Keep the payload sizes consistent across exploit attempts.

`Payloads`: Code runs remotely and calls back to the attacker machine to establish a connection (or shell).

`Plugins`: Additional scripts can be integrated within an assessment with `msfconsole` and coexist.

`Post`: Wide array of modules to gather information, pivot deeper, etc.

Only `Auxiliary`, `Exploits` and `Post` can use `use`.
```bash
msf> use exploit/...
```

### Using Modules
To check which options are needed to be set before the exploit can be sent to the target host, we can use the `show options` command.
```bash
<SNIP>

Matching Modules
================

   #  Name                                  Disclosure Date  Rank    Check  Description
   -  ----                                  ---------------  ----    -----  -----------
   0  exploit/windows/smb/ms17_010_psexec   2017-03-14       normal  Yes    MS17-010 EternalRomance/EternalSynergy/EternalChampion SMB Remote Windows Code Execution
   1  auxiliary/admin/smb/ms17_010_command  2017-03-14       normal  No     MS17-010 EternalRomance/EternalSynergy/EternalChampion SMB Remote Windows Command Execution
   
   
msf > use 0
msf exploit(windows/smb/ms17_010_psexec) > options
```

### Module Information
We can use the command info after selecting the module if we want to know something more about the module.
```bash
msf exploit(windows/smb/ms17_010_psexec) > info
```


