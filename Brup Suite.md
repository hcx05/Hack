
---

Brup serve as a proxy server, we use Brup proxy to observe and modify the HTTP request responses. There are three types of proxy : Foward proxy, Reverse proxy and transparent proxy.

Use the command to check  listing ports status in Windows
```
netstat -ant

TCP    127.0.0.1:8080         0.0.0.0:0              LISTENING       InHost
```

We can serve `127.0.0.1` as a Virtual Network Adapter

Public Key Infrastructure(PKI) : 
	[[Symmetric encryption]] 
	[[Asymmetric encryption]] 
	
Assume there is a man-in-the-middle (MITM) attacker between the server and the user.

When the server sends its public key **A** to the user, the attacker intercepts the transmission.  
Instead of forwarding the server’s public key, the attacker generates their own public–private key pair **B** and sends the attacker’s public key **B** to the user.

The user receives public key **B** and has no way to know that this key does not actually belong to the server.  The user then encrypts sensitive data using public key **B** and sends it to the server.

The attacker intercepts the encrypted data again and uses the attacker’s private key **B** to decrypt it.  At this point, the attacker can read the data or even modify its contents.

After that, the attacker re-encrypts the data using the server’s real public key **A** and forwards it to the server.  The server then decrypts the data using its private key **A**, believing the communication is secure.This entire process is an example of a **man-in-the-middle attack in asymmetric encryption**.

To prevent this type of attack, [[Digital certificates ]]are used.  
Certificates allow the user to verify that a public key truly belongs to the intended server, preventing attackers from substituting their own keys.
