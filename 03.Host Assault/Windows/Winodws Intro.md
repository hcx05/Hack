
---
### Filesystems
#### 1. FAT
The File Allocation Table, is not the default file system for Microsoft Windows operating systems today. Today, it is used in USB flash drives, flash drives, and other embedded systems.

#### 2.exFAT
Extensible File Allocation Table, a file system introduced by Microsoft in 2006 and optimized for flash memory such as USB flash drives and SD cards.

#### 3. NTFS
New Technology File system, announced and started to be used with the Windows NT 3.1 version in 1993. It was used as the default file system within the Windows operating system with this version and it still is.
### Directory Structure
#### Root Directory
A drive, usually `C:\`. The directory separator is usually a `"\"`, but the operating system also internally recognizes a `"/"`. Physical and virtual drives are named by a drive letter, as opposed to being combined as one.

#### PerfLogs
It is the folder created to keep Windows performance logs. It is found as an empty folder because the logging option is turned off by default.

#### Program Files
All the programs installed in the system are located under the "Program Files" folder in a Windows operating system installed as 32-bit. In Windows operating systems installed as 64-bit, only 64-bit programs are installed under this folder.

#### Program Files(x86)
Only 64-bit OS has this file. Under this folder, programs installed as "32-bit" are stored.

#### Users
The Users folder contains the personal folder of each user who has logged on to the system at least once. Folders and documents such as desktop folder, downloaded files, and documents are stored under this folder that belongs to each user on the system.

#### Windows
The Windows folder is where the entire operating system is installed. It has its own structure and it contains many systemic information in a certain order. For example, the database where users' passwords are kept is located under this folder.

### Command Line
`cls`, `rename`, `copy`, `move`, `type`(`cat` in linux), `rmdir`
#### NET
`net user`: List all the users.

`net accounts`: Allows users to see the configurations related to password usage and logon restrictions on the system.

`net localgroup`: When used without parameters, it only displays the list of groups on the system. Want to see the information of a group in the system, the group name should be added as a parameter to the "net localgroup" command.
### Users and Groups
`lusrmgr.msc`: Can manage users and groups.






















