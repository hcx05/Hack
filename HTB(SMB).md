
---
SMB : Server Message Block, a protocol to transfer a file between two hosts(computers) on the same network. (Mostly see SMB services running on Windows machines.)

SMB usually operate on port 445

Usually, SMB runs at the Application or Presentation layers of the OSI model, pictured below. Due to this, it relies on lower-level protocols for transport.

An SMB-enabled storage on the network is called a share . These can be accessed by any client that has the address of the server and the proper credentials.

If we do not specify a specific username to smbclient when attempting to connect to the remote host, it will just use your local machine's username.

```jsx
smbclient -L {target_IP} 
// [-L|--list=HOST] : Selecting the targeted host for the connection request.
```

```jsx
smbclient {target_IP} {File_Name}
```