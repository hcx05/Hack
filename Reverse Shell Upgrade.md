
---
TTY(Teletype) represent a intact terminal in Linux.

Use Python to spawn a pty.The pty module let’s you spawn a psuedo-terminal that can fool commands like `su` into thinking they are being executed in a proper terminal.

```bash
python -c 'import pty; pty.spawn("/bin/bash")'
```
