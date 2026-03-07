
---
TTY(Teletype) represent a intact terminal in Linux.

Use Python to spawn a pty.The pty module let’s you spawn a psuedo-terminal that can fool commands like `su` into thinking they are being executed in a proper terminal.

```bash
python -c 'import pty; pty.spawn("/bin/bash")'
```

Now, suspend the shell and return control to local terminal by pressing:

```
Ctrl + Z
```

This moves the shell to the background, allowing us to modify terminal settings.

Run the following command to prepare the terminal for better interaction:

```bash
stty raw -echo && fg
```

After running `reset`, the terminal may prompt for a terminal type. Enter:

```bash
xtrem
```

If xtrem not found `Cntl+C` to suspend the reset process.

```bash
export TERM=linux
export SHELL=bash
```

This ensures that the shell recognizes arrow keys, tab completion, and interactive commands.