
---
WMI: Windows Management Instrumentation. The initialization of the WMI communication always takes place on `TCP` port `135`, and after the successful establishment of the connection, the communication is moved to a random port.

WMI allows read and write access to almost all settings on Windows systems.

WMI is not a single program but consists of several programs and various databases, also known as repositories.

#### WMIexec.py
```bash
hcx05@htb[/htb]$ /usr/share/doc/python3-impacket/examples/wmiexec.py Cry0l1t3:"P455w0rD!"@10.129.201.248 "hostname"

Impacket v0.9.22 - Copyright 2020 SecureAuth Corporation

[*] SMBv3.0 dialect used
ILF-SQL-01
```