
---
WinRM: Windows Remote Management, a protocol based on CLI.

WinRM uses the Simple Object Access Protocol (`SOAP`) to establish connections to remote hosts and their applications.
WinRM relies on `TCP` ports `5985` and `5986` for communication, with the last port `5986 using HTTPS`, as ports 80 and 443 were previously used for this task. However, since port 80 was mainly blocked for security reasons, the newer ports 5985 and 5986 are used today.

WinRS(Windows Remote Shell) is an administrative component of WinRM, which let us execute arbitrary commands on the remote system.

### Test if it's reachable via WinRM
Powershell uses [Test-WsMan](https://docs.microsoft.com/en-us/powershell/module/microsoft.wsman.management/test-wsman?view=powershell-7.2), while Linux uses  [evil-winrm](https://github.com/Hackplayers/evil-winrm).
```bash
hcx05@htb[/htb]$ evil-winrm -i 10.129.201.248 -u Cry0l1t3 -p P455w0rD!

Evil-WinRM shell v3.3

Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine

Data: For more information, check Evil-WinRM Github: https://github.com/Hackplayers/evil-winrm#Remote-path-completion

Info: Establishing connection to remote endpoint

*Evil-WinRM* PS C:\Users\Cry0l1t3\Documents>
```