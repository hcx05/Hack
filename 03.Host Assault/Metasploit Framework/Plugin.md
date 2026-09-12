
---
Navigating to `/usr/share/metasploit-framework/plugins`, which is the default directory for every new installation of `msfconsole`.
```bash
hcx05@htb[/htb]$ ls /usr/share/metasploit-framework/plugins

aggregator.rb      beholder.rb        event_tester.rb  komand.rb     msfd.rb    nexpose.rb   request.rb  session_notifier.rb  sounds.rb  token_adduser.rb  wmap.rb
alias.rb           db_credcollect.rb  ffautoregen.rb   lab.rb        msgrpc.rb  openvas.rb   rssfeed.rb  session_tagger.rb    sqlmap.rb  token_hunter.rb
auto_add_route.rb  db_tracker.rb      ips_filter.rb    libnotify.rb  nessus.rb  pcap_log.rb  sample.rb   socket_logger.rb     thread.rb  wiki.rb
```
#### MSF - Load Nessus
```bash
msf6 > load nessus

[*] Nessus Bridge for Metasploit
[*] Type nessus_help for a command listing
[*] Successfully loaded Plugin: Nessus


msf6 > nessus_help

Command                     Help Text
-------                     ---------
Generic Commands            
-----------------           -----------------
nessus_connect              Connect to a Nessus server
nessus_logout               Logout from the Nessus server
nessus_login                Login into the connected Nessus server with a different username and 

<SNIP>

nessus_user_del             Delete a Nessus User
nessus_user_passwd          Change Nessus Users Password
                            
Policy Commands             
-----------------           -----------------
nessus_policy_list          List all polciies
nessus_policy_del           Delete a policy
```
### Installing new Plugins
To install new custom plugins not included in new updates of the distro, we can take the .rb file provided on the maker's page and place it in the folder at `/usr/share/metasploit-framework/plugins`.
```bash
git clone https://github.com/darkoperator/Metasploit-Plugins
```
### popular plugins
||||
|---|---|---|
|[nMap (pre-installed)](https://nmap.org/)|[NexPose (pre-installed)](https://sectools.org/tool/nexpose/)|[Nessus (pre-installed)](https://www.tenable.com/products/nessus)|
|[Mimikatz (pre-installed V.1)](http://blog.gentilkiwi.com/mimikatz)|[Stdapi (pre-installed)](https://www.rubydoc.info/github/rapid7/metasploit-framework/Rex/Post/Meterpreter/Extensions/Stdapi/Stdapi)|[Railgun](https://github.com/rapid7/metasploit-framework/wiki/How-to-use-Railgun-for-Windows-post-exploitation)|
|[Priv](https://github.com/rapid7/metasploit-framework/blob/master/lib/rex/post/meterpreter/extensions/priv/priv.rb)|[Incognito (pre-installed)](https://www.offensive-security.com/metasploit-unleashed/fun-incognito/)|[Darkoperator's](https://github.com/darkoperator/Metasploit-Plugins)|