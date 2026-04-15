
---
About /etc file : Inside etc directory, we will get three most important files i.e. **passwd**, **group**, and **shadow**.

**etc/passwd:** It is a human-readable text file which stores information of user account.

**etc/group:** It is also a human-readable text file which stores group information as well as user belongs to which group can be identified through this file.

**etc/shadow:** It is a file that contains encrypted password and information of the account expire for any user.

`/etc/passwd` format :

`username:password:UID:GID:comment:home:shell`

**Username:** The First field indicates the name of the user, which is used to login.

**Encrypted password:** The **X denote**s an encrypted password, which is stored inside /shadow file. If the user does not have a password, the password field will display an asterisk *.

**User Id (UID):** Every user must receive a user ID (UID). The system keeps UID 0 (zero) for the root user, assigns UIDs 1-99 for further predefined accounts, and allocates UIDs 100-999 for administrative purposes. The system designates UID 1000 as almost always the first non-system user, usually an ***administrator***. If we create a new user on our Ubuntu system, it will receive the UID of 1001.

**Group Id (GID):** It denotes the group of each user; like as UIDs, the first **100** GIDs are usually kept for system use. The GID of **0** relates to the root group and the GID of **1000** usually signifies the users. New groups are generally allotted GIDs beginning from **1000.**

**Gecos Field:** Usually, this is a set of comma-separated values that tells more details related to the users. The format for the GECOS field denotes the following information:

User’s full name, Building and room number or contact person, Office telephone number

**Shell:** It denotes the full path of the default shell that executes the command (by the user) and displays the results.

If /etc/passwd file is having -rwxrwxrwx permissions in victim system, open /etc/passwd file and remove the ‘x’ value at the place of root password.