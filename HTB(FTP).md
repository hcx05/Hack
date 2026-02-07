
---
FTP : File Transfer Protocol, a native protocol to all host operating systems and used for a long time for simple file transfer tasks.

FTP is built on a client–server model architecture using separate control and data connections between the client and the server.

FTP is often secured with SSL/TLS (FTPS) or replaced with SSH File Transfer Protocol (SFTP).

MitM : Men-in-the-Middle Attack

FTP service usually listen on port 21.

```jsx
ftp -y //The -y switch at the end of the command is used to accept the installation without interrupting the process
//to ask you if you'd like to proceed.
```

```jsx
ftp -? //see what the service is capable of.
```

Username ‘anonymous’ is used over FTP when you want to log in without having an account.

230 is the response code when “Login successful”

```jsx
get {terget_file}
```
