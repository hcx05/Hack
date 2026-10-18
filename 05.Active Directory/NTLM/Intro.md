
---
LM and NT are hash algorithms used to store password credentials. NTLMv1 and NTLMv2 are authentication protocols built on top of these hashes.

NTLMv1 can use either LM or NT hashes, while NTLMv2 exclusively uses NT hashes.
### LM
LAN Manager: Oldest password storage mechanism used by the Windows operating system. If in use, they are stored in the SAM database on a Windows host and the NTDS.DIT database on a Domain Controller.
#### Alg
If the password is less than fourteen characters, it will be padded with NULL characters to reach the correct value. Two DES keys are created from each chunk. These chunks are then encrypted using the string `KGS!@#$%`, creating two 8-byte ciphertext values. These two values are then concatenated together, resulting in an LM hash.
### NTHash (NTLM)
NTML: NT LAN Manager, used on modern Windows systems. It is a challenge-response authentication protocol and uses three messages to authenticate(kinda like t3 way handshake).

These hashes are stored locally in the SAM database or the NTDS.DIT database file on a Domain Controller. The protocol has two hashed password values to choose from to perform authentication: the LM hash (as discussed above) and the NT hash, which is the MD4 hash of the little-endian UTF-16 value of the password.
#### NTLM Authentication Request
![](assets/Pasted%20image%2020261017161501.png)
#### Example
```bash
Rachel:500:aad3c435b514a4eeaad3b935b51304fe:e46b9e548fa0d122de7f59fb6d48eaa2:::
```
- `Rachel` is the username
- `500` is the Relative Identifier (RID). 500 is the known RID for the `administrator` account
- `aad3c435b514a4eeaad3b935b51304fe` is the LM hash and, if LM hashes are disabled on the system, can not be used for anything
- `e46b9e548fa0d122de7f59fb6d48eaa2` is the NT hash. This hash can either be cracked offline to reveal the cleartext value (depending on the length/strength of the password) or used for a pass-the-hash attack.
**Note:** Neither LANMAN nor NTLM uses a salt.
### NTLMv1 (Net-NTLMv1)
The NTLM protocol performs a challenge/response between a server and client using the NT hash.

NTLMv1 uses both the NT and the LM hash, which can make it easier to "crack" offline after capturing a hash using a tool such as [Responder](https://github.com/lgandx/Responder) or via an NTLM relay attack.

The protocol is used for network authentication. These hashes can NOT be used for pass-the-hash attacks.
#### V1 Challenge & Response Algorithm
The Net-NTLMv1 hash itself is created from a challenge/response algorithm. The server sends the client an 8-byte random number (challenge), and the client returns a 24-byte response.
```bash
C = 8-byte server challenge, random
K1 | K2 | K3 = LM/NT-hash | 5-bytes-0
response = DES(K1,C) | DES(K2,C) | DES(K3,C)
```
#### Example
```bash
u4-netntlm::kNS:338d08f8e26de93300000000000000000000000000000000:9526fb8c23a90751cdd619b6cea564742e1e4bf33006ba41:cb8086049ec4736c
```
### NTLMv2 (Net-NTLMv2)
It has been the default in Windows since Server 2000. It is hardened against certain spoofing attacks that NTLMv1 is susceptible to.
#### V2 Challenge & Response Algorithm
NTLMv2 sends two responses to the 8-byte challenge received by the server. These responses contain a 16-byte HMAC-MD5 hash of the challenge, a randomly generated challenge from the client, and an HMAC-MD5 hash of the user's credentials. A second response is sent, using a variable-length client challenge including the current time, an 8-byte random value, and the domain name.
```bash
SC = 8-byte server challenge, random
CC = 8-byte client challenge, random
CC* = (X, time, CC2, domain name)
v2-Hash = HMAC-MD5(NT-Hash, user name, domain name)
LMv2 = HMAC-MD5(v2-Hash, SC, CC)
NTv2 = HMAC-MD5(v2-Hash, SC, CC*)
response = LMv2 | CC | NTv2 | CC*
```
#### Example
```bash
admin::N46iSNekpT:08ca45b7d7ea58ee:88dcbe4446168966a153a0064958dac6:5c7830315c7830310000000000000b45c67103d07d7b95acd12ffa11230e0000000052920b85f78d013c31cdb3b92f5d765c783030
```
### Hash Protocol Comparison

|**Hash/Protocol**|**Cryptographic technique**|**Mutual Authentication**|**Message Type**|**Trusted Third Party**|
|---|---|---|---|---|
|`NTLM`|Symmetric key cryptography|No|Random number|Domain Controller|
|`NTLMv1`|Symmetric key cryptography|No|MD4 hash, random number|Domain Controller|
|`NTLMv2`|Symmetric key cryptography|No|MD4 hash, random number|Domain Controller|
|`Kerberos`|Symmetric key cryptography & asymmetric cryptography|Yes|Encrypted ticket using DES, MD5|Domain Controller/Key Distribution Center (KDC)|



























