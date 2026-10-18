
---
Kerberos has been the default authentication protocol for domain accounts since Windows 2000. Kerberos is an open standard and allows for interoperability with other systems using the same standard.

When a user logs into their PC, Kerberos is used to authenticate them via mutual authentication, or both the user and the server verify their identity.

Kerberos is a stateless authentication protocol based on tickets instead of transmitting user passwords over the network.

The Kerberos protocol uses port 88 (both TCP and UDP).
#### Kerberos Authentication Process

||
|---|
|1. When a user logs in, their password is used to encrypt a timestamp, which is sent to the Key Distribution Center (KDC) to verify the integrity of the authentication by decrypting it. The KDC then issues a Ticket-Granting Ticket (TGT), encrypting it with the secret key of the krbtgt account. This TGT is used to request service tickets for accessing network resources, allowing authentication without repeatedly transmitting the user's credentials. This process decouples the user's credentials from requests to resources.|
|2. The KDC service on the DC checks the authentication service request (AS-REQ), verifies the user information, and creates a Ticket Granting Ticket (TGT), which is delivered to the user.|
|3. The user presents the TGT to the DC, requesting a Ticket Granting Service (TGS) ticket for a specific service. This is the TGS-REQ. If the TGT is successfully validated, its data is copied to create a TGS ticket.|
|4. The TGS is encrypted with the NTLM password hash of the service or computer account in whose context the service instance is running and is delivered to the user in the TGS_REP.|
|5. The user presents the TGS to the service, and if it is valid, the user is permitted to connect to the resource (AP_REQ).|

![](assets/Pasted%20image%2020261017133508.png)