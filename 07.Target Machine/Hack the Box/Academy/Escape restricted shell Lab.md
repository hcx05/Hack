---
tags:
    #rbash
    #shell_escape
    #SSH
    #bash_profile
---

## 1. Initial Enumeration

### Identifying the Restricted Shell
Upon logging in, we find ourselves in a restricted environment. Attempts to use absolute paths or common commands are blocked.

```bash
htb-user@ubuntu:~$ ls
bash: /usr/lib/command-not-found: restricted: cannot specify `/' in command names
```

The error message indicates we are in a restricted shell (`rbash`), which prevents the use of `/` in command names, among other restrictions.

### Checking the Environment
We check the `$PATH` to see what commands are available to us.

```bash
htb-user@ubuntu:~$ echo $PATH
/home/htb-user/bin

htb-user@ubuntu:~$ echo /home/htb-user/bin/*
/home/htb-user/bin/man
```

The only command available in our path is `man`. However, even `man` is broken because it relies on `cat`, which is not in our restricted path.

```bash
htb-user@ubuntu:~$ man man
man: can't execute cat: No such file or directory
```

## 2. Reading Files via Bash Built-ins

Since we cannot execute most binaries, we can try to use Bash built-in commands, which are executed by the shell itself and often bypass path restrictions.

We can read the `flag.txt` file using a `while` loop and the `read` built-in.

```bash
htb-user@ubuntu:~$ while read line; do echo $line; done < flag.txt
HTB{35c4p3_7h3_r3stricted_5h311}
```

## 3. Escaping the Restricted Shell

### Method 1: Interrupting Profile Loading via Broken Bash
During the escape attempt, we discovered that the `bash` binary on this system is "broken"—it spawns but is completely non-responsive to input.

1. **Execution:** We attempt to log in using `ssh htb-user@htb -t "bash --noprofile"`.
2. **Observation:** The shell hangs. Commands like `ls` or `cd` produce no output and no reaction.
3. **The Escape:** Since the child `bash` process is unresponsive, we must press **`^C` (Ctrl+C)** to kill it.
4. **The Result:** Killing the broken child process returns us to the parent `rbash` shell. Crucially, because the initialization sequence was interrupted by the broken process and our subsequent `^C`, **the shell skips the execution of restrictive profile scripts** (like `.bash_profile` or `/etc/profile`). 

We are returned to an `rbash` session, but without the restricted `$PATH`, granting us full access to system commands.

> **Clarification on `--noprofile`:** In this specific scenario, the `--noprofile` flag is actually optional or even irrelevant. Since the `bash` process is broken and non-interactive from the start, it never reaches the point of processing profiles normally anyway. The escape relies entirely on the interruption of the shell's initialization flow.

### Method 2: Verifying the Hypothesis via `sh` (Final Escape)
To confirm that the restrictions were tied to the Bash environment and that the `bash` binary itself was the issue, we tried logging in directly with `sh`:

```bash
ssh htb-user@htb -t "sh"

$ echo $SHELL
sh
```

**Result:** The login via `sh` worked perfectly and was completely unrestricted. This confirms that the security limitations (and the "broken" behavior) were specific to the Bash initialization flow. By using `sh`, we bypass the broken `bash` binary and the restrictive profiles entirely.

---

## Additional Knowledge & Concept Clarification

### What is rbash?
`rbash` is a restricted version of the Bash shell. When Bash is started with the name `rbash`, or with the `-r` option, it becomes restricted. Key restrictions include:
- Changing directories with `cd`.
- Setting or unsetting the values of `SHELL`, `PATH`, `ENV`, or `BASH_ENV`.
- Specifying command names containing `/`.
- Redirecting output using `>`, `>|`, `<>`, `>&`, `&>`, and `>>`.

### How Shell Profiles Work
When you log in via SSH, the shell is typically a "login shell." It looks for and executes commands from several files in order:
1. `/etc/profile`
2. `~/.bash_profile`
3. `~/.bash_login`
4. `~/.profile`

Restrictions are often placed in `~/.bash_profile` to set a limited `PATH` and make it read-only (`readonly PATH`).

### Why SSH -t Works
The `-t` option forces pseudo-terminal allocation, which is necessary for interactive shells. By providing a command like `"sh"` or `"bash --noprofile"`, we instruct SSH to execute that specific command instead of the default login shell sequence. If the system allows it, this can bypass the loading of the restrictive profile files entirely.
