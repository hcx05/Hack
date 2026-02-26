
---
Special permission is given to each user it becomes **SUID, SGID, and sticky bits**. When extra bit **“4”** is set to user(Owner) it becomes **SUID** (Set user ID) and when bit **“2”** is set to group it becomes **SGID** (Set Group ID) and  if other users are allowed to create or delete any file inside a directory then **sticky bits** **“1”** is set to that directory.

```
 | rwx | rwx | rwx
    ↑     ↑     ↑
 | rws | rws | rwt
```

**SUID:** Set User ID is a type of permission that allows users to execute a file with the permissions of a specified user. Those files which have suid permissions run with higher privileges.  Assume we are accessing the target system as a non-root user and we found suid bit enabled binaries, then those file/program/command can run with root privileges.

Find SIUD Files : 

```bash
find / -perm -u=s -type f 2>/dev/null # replace = to + is fine.
```

Some famous Linux / Unix executable commands that can allow privilege escalation: bash, cat, cp, echo, find, less, more, nano, nmap, Vim and etc.

