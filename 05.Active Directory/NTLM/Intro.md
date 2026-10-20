
---
LM and NT are hash algorithms used to store password credentials. NTLMv1 and NTLMv2 are authentication protocols built on top of these hashes.

NTLMv1 can use either LM or NT hashes, while NTLMv2 exclusively uses NT hashes.
### LM
LAN Manager: Oldest password storage mechanism used by the Windows operating system. If in use, they are stored in the SAM database on a Windows host and the NTDS.DIT database on a Domain Controller.
#### Alg
If the password is less than fourteen characters, it will be padded with NULL characters to reach the correct value. Two DES keys are created from each chunk. These chunks are then encrypted using the string `KGS!@#$%`, creating two 8-byte ciphertext values. These two values are then concatenated together, resulting in an LM hash.
### NTHash (NTLM)
NTML: NT LAN Manager([MS-NLMP](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/b38c36ed-2804-4868-a9ff-8dd3182128e4)), used on modern Windows systems to authenticate remote users and optionally provide session security when requested by the application. It is a challenge-response authentication protocol that use `nonces`(pseudo-random numbers generated for one-time use), using three messages to authenticate(kinda like 3 way handshake).

The `NTLM` security protocols are all embedded protocols, meaning that although `NTLM` has messages and a state machine like other protocols, it does not have a network protocol stack layer.

The hashes are stored locally in the SAM database or the NTDS.DIT database file on a Domain Controller. The protocol has two hashed password values to choose from to perform authentication: the LM hash (as discussed above) and the NT hash, which is the MD4 hash of the little-endian UTF-16 value of the password.

- The `NTLM` version used on hosts, whether `NTLMv1` or `NTLMv2`, is [configured out-of-band](https://learn.microsoft.com/en-us/windows/security/threat-protection/security-policy-settings/network-security-lan-manager-authentication-level) before authentication.
- Using a secure mechanism, the client and server/DC share a secret key (the user's password's hash) before authentication.
- Neither plaintext credentials nor the shared secret key are sent over the wire.
#### SSPI
[Security Support Provider Interface](https://learn.microsoft.com/en-us/windows/win32/rpc/sspi-architectural-overview) (`SSPI`), the foundation of Windows authentication, is an API that allows connected applications to call one of several security providers to establish authenticated connections and to exchange data securely over those connections. A [security support provider](https://learn.microsoft.com/en-us/windows/win32/rpc/security-support-providers-ssps-) (`SSP`) is a dynamic-link library (`DLL`) responsible for implementing the `SSPI` by exposing one or more security packages to applications; each security package provides mappings between an application's `SSPI` function calls and an actual security model's functions. These security packages support various security protocols, including [NTLM](https://learn.microsoft.com/en-us/windows/win32/secauthn/microsoft-ntlm).

[NTLM SSP](https://learn.microsoft.com/en-us/windows-server/security/windows-authentication/security-support-provider-interface-architecture#BKMK_NTLMSSP) (located at `%Windir%\System32\msv1_0.dll`) is a binary messaging protocol utilized by `SSPI` to facilitate `NTLM` challenge-response authentication and to negotiate options for integrity and confidentiality. The `NTLM SSP` encompasses both the `NTLM` and `NTLMv2` authentication protocols.
#### NTLM Authentication Request
The `NTLM` authentication workflow for domain-joined computers:
![](assets/Pasted%20image%2020261018125454.png)
Once it receives the `AUTHENTICATE_MESSAGE`, and because it does not possess the client's secret key, the server delegates the verification of the user's identity to a DC (a procedure known as [Pass-through authentication](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nrpc/70697480-f285-4836-9ca7-7bb52f18c6af)) by invoking [NetrLogonSamLogonWithFlags](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nrpc/d17f1077-de4b-4fcd-8867-39068cb789f5), which contains [NETLOGON_NETWORK_INFO](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nrpc/e17b03b8-c1d2-43a1-98db-cf8d05b9c6a8), a data structure populated with the various fields that the DC requires to verify the user.

The only difference for workgroup authentication is that the server verifies the user's identity instead of delegating it to the DC:
![](assets/Pasted%20image%2020261018130432.png)
### NTLM Messages
The headers:
- Signature: 8 byte string `NTLMSSP\0`, to make clear this a NTLM messgae.
- MessageType: 1, 2 or 3
![](assets/Pasted%20image%2020261018131437.png)

|Field|Meaning|
|:--|:--|
|`Signature`|An 8-byte NULL-terminated ASCII string always set to [`N`, `T`, `L`, `M`, `S`, `S`, `P`, `\0`].|
|`MessageType`|A 4-byte unsigned integer always set to either `0x00000001` (`NtLmNegotiate`) to indicate that the `NTLM` message is a `NEGOTIATE_MESSAGE` or `0x00000002` (`NtLmChallenge`) to indicate that the `NTLM` message is a `CHALLENGE_MESSAGE` or `0x00000003` (`NtLmAuthenticate`) to indicate that the `NTLM` message is an `AUTHENTICATE_MESSAGE`.|
|`MessageDependentFields`|A variable-length field that contains the `NTLM` message contents.|
|`payload`|A variable-length field that contains a message-dependent number of individual payload messages, referenced by byte offsets in `MessageDependentFields`.|
#### NEGOTIATE_MESSAGE
The [NEGOTIATE_MESSAGE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/b34032e5-3aae-4bc6-84c3-c6d80eadf7f2) is the first `NTLM`-specific message, it contains four message-dependent fixed-length fields. One important field to know about is `NegotiateFlags`; this 4-bytes field, present in all three `NTLM` messages and not exclusive to `NEGOTIATE_MESSAGE`, is a [NEGOTIATE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/99d90ff4-957f-4c8a-80e4-5bfe5a9a9832) structure consisting of 32 1-bit flags that allow indicating which `NTLM` capabilities are supported/requested by the sender.
#### CHALLENGE_MESSAGE
The [CHALLENGE_MESSAGE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/801a4681-8809-4be9-ab0d-61dcfe762786) is the second `NTLM`-specific message, it contains six message-dependent fixed-length fields, two important to know about, `NegotiateFlags` and `ServerChallenge`. `NegotiateFlags` holds the flags the server has chosen from the options offered/requested by the client in `NegotiateFlags` of the `NEGOTIATE_MESSAGE`. At the same time, `ServerChallenge` is a 64-bit `nonce` that holds the `NTLM` challenge generated by the server.

[NTLM Challenger](https://github.com/nopfor/ntlm_challenger), [ntlm-info](https://gitlab.com/Zer1t0/ntlm-info), [NTLMRecon](https://github.com/praetorian-inc/NTLMRecon), and [DumpNTLMInfo.py](https://github.com/fortra/impacket/blob/impacket_0_11_0/examples/DumpNTLMInfo.py) perform reconnaissance against endpoints/hosts that accept `NTLM` authentication by parsing the information returned within the `CHALLENGE_MESSAGE`.
```bash
hcx05@htb[/htb]$ python3 examples/DumpNTLMInfo.py 172.16.117.3

Impacket v0.12.0.dev1+20230803.144057.e2092339 - Copyright 2023 Fortra

[+] SMBv1 Enabled   : False
[+] Prefered Dialect: SMB 3.0
[+] Server Security : SIGNING_ENABLED | SIGNING_REQUIRED
[+] Max Read Size   : 8.0 MB (8388608 bytes)
[+] Max Write Size  : 8.0 MB (8388608 bytes)
[+] Current Time    : 2023-08-14 17:39:26.822236+00:00
[+] Name            : DC01
[+] Domain          : INLANEFREIGHT
[+] DNS Tree Name   : INLANEFREIGHT.LOCAL
[+] DNS Domain Name : INLANEFREIGHT.LOCAL
[+] DNS Host Name   : DC01.INLANEFREIGHT.LOCAL
[+] OS              : Windows NT 10.0 Build 17763
[+] Null Session    : True
```
#### AUTHENTICATE_MESSAGE
The [AUTHENTICATE_MESSAGE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/033d32cc-88f9-4483-9bf2-b273055038ce) is the third and last `NTLM`-specific message, it contains nine message-dependent fixed-length fields, two important to know about, `LmChallengeResponseFields` and `NtChallengeResponseFields`(Answers).
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
#### LM response
If `NTLMSSP_NEGOTIATE_LM_KEY` was set and `NTLMv1` is used, `LmChallengeResponseFields`(field in `AUTHENTICATE_MESSAGE`) contains a [LM_RESPONSE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/e3fee6d1-0d93-4020-84ab-ca4dc5405fc9) structure.

- `NTLMSSP_NEGOTIATE_LM_KEY`: `G` bit in `NegotiateFlags`(field in `NEGOCIATE_MESSAGE`), client tells the server it support and want to caculate the `LM` hash then resopnse.

`LM_RESPONSE` contains one field, which is `Response`, a 24-byte array of [unsigned char](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/050baef1-f978-4851-a3c7-ad701a90e54a) that contains the client's `LmChallengeResponse`.

To compute the `Response` field of `LM_RESPONSE` or `LMv2_RESPONSE`, `MS-NLMP` provides [pseudocode](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/1b72429a-d8b8-4a04-bc82-1eedc980b87a).

[LMOWFv1](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/780943e9-42e6-4dbe-aa87-1dce828ba82a#gt_fd74ef50-cb97-4acd-b537-4941bdd9e064) (implemented in the [compute_lmhash](https://github.com/fortra/impacket/blob/9a8d27034eab20d23802730d0c69bf99356d8af1/impacket/ntlm.py#L742C1-L747) function), used only by `LM` and `NTLMv1`, is an `NT LAN Manager` (`LM`) one-way function that also creates a hash based on the user's password to generate a principal's security key. The client uses these two functions to calculate the response it returns to the server; the [pseudocode](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/464551a8-9fc4-428e-b3d3-bc5bfb2e73a5) for the response calculation is implemented in the function [computeResponseNTLMv1](https://github.com/fortra/impacket/blob/9a8d27034eab20d23802730d0c69bf99356d8af1/impacket/ntlm.py#L717-L740).
#### NT response
[NTOWFv1](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/780943e9-42e6-4dbe-aa87-1dce828ba82a#gt_7a2805fa-1dcd-4b4e-a8e4-2a2bcc8651e9)(used only by `NTLMv1` and implemented in the [compute_nthash](https://github.com/fortra/impacket/blob/9a8d27034eab20d23802730d0c69bf99356d8af1/impacket/ntlm.py#L759-L769) function), an one-way function of `LAN Manager`, which creates a NT hash(MD4) based on the user's password to generate a principal's security key.

For `NtChallengeResponseFields`, if `NTLMv1` is used, `NtChallengeResponse` will contain an [NTLM_RESPONSE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/b88739c6-1266-49f7-9d22-b13923bd8d66) structure,
`NTLM_RESPONSE` contains one field, which is `Response`, a 24-byte array of unsigned char that contains the client's `NtChallengeResponse`.
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

`NTLMv2` provides three primary operations:

1. Authentication.
2. Message integrity, known as message `signing` in the `NTLM` terminology.
3. Message confidentiality, known as message `sealing` in the `NTLM` terminology.

If `NTLMSSP_NEGOTIATE_LM_KEY` was agreed and use `NTLMv2`,  `LmChallengeResponseFields` will contain a [LMv2_RESPONSE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/8659238f-f5a9-44ad-8ee7-f37d3a172e56) structure. It contains two fields, `Response` and `ChallengeFromClient`; `Response` is a 16-byte array of unsigned char that contains the clients `LM` `challenge-response`, while `ChallengeFromClient` is an 8-byte array of unsigned char that contains a challenge generated by the client.

For the response calculation of [NTLMv2 authentication](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/5e550938-91d4-459f-b67d-75d70009e3f3), [NTOWFv2](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/780943e9-42e6-4dbe-aa87-1dce828ba82a#gt_ba118c39-b391-4232-aafa-a876ee1e9265) and [LMOWFv2](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/780943e9-42e6-4dbe-aa87-1dce828ba82a#gt_a043ea96-e876-4259-be4b-aa8d2335fdfe) (both are version-dependent and only used by `NTLMv2`, they are implemented in the functions [LMOWFv2](https://github.com/fortra/impacket/blob/9a8d27034eab20d23802730d0c69bf99356d8af1/impacket/ntlm.py#L896C1-L897) and [NTOWFv2](https://github.com/fortra/impacket/blob/9a8d27034eab20d23802730d0c69bf99356d8af1/impacket/ntlm.py#L889C1-L894), respectively) are the one-way functions used to create a hash based on the user's password to generate a principal's security key. With these two functions, the client calculates the response it returns to the server as described in [pseudocode](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/5e550938-91d4-459f-b67d-75d70009e3f3) (implemented in the function [computeResponseNTLMv2](https://github.com/fortra/impacket/blob/9a8d27034eab20d23802730d0c69bf99356d8af1/impacket/ntlm.py#L900-L937)).

For `NtChallengeResponseFields`, if `NTLMv2` is used, then `NtChallengeResponse` will contain a [NTLMv2_RESPONSE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/d43e2224-6fc3-449d-9f37-b90b55a29c80) structure, it contains two fields, `Response` and a [NTLMv2_CLIENT_CHALLENGE](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/aee311d6-21a7-4470-92a5-c4ecb022a87b) structure; `Response` is a 16-byte array of unsigned char that contains the client's `NtChallengeResponse`, while `NTLMv2_CLIENT_CHALLENGE` is a variable-length byte array that contains eight fixed-length variables, including `ChallengeFromClient`.
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
### NTLM Session Security
If the client and server negotiate it, [session security](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/d1c86e81-eb66-47fd-8a6f-970050121347) provides [message integrity](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/131b0062-7958-460e-bca5-c7a9f9086652) (`signing`) and [message confidentiality](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-nlmp/115f9c7d-bc30-4262-ae96-254555c14ea6) (`sealing`). SSPI provides session security.

`NTLMv1` only provides `signing`.
#### Signing and Sealing
When `session signing` is negotiated, the client and server negotiate a `session key` to sign all messages exchanged. The `session key` is generated using a combination of the client's and server's challenge messages and the user's password hash.

Once the session key is established, all messages between the client and server are signed using a `MAC`. The `MAC` is generated by applying a cryptographic algorithm to the message and the session key. The server can verify the `MAC` by using the same algorithm as the message and the session key and comparing the result to the `MAC` provided by the client.

#### SMB signing
Default `SMB signing` settings for hosts in the network depending on the SMB version they are running. Except for `SMB1`, which has three possible settings, `Required`, `Enabled`, or `Disabled`, `SMB2` and `SMB3` only have `Required` or `Not Required`:

| **Host**              | **Default Signing Setting** |
| --------------------- | --------------------------- |
| `SMB1 Client`         | `Enabled`                   |
| `SMB1 Server`         | `Disabled`                  |
| `SMB2 & SMB3 Clients` | `Not Required`              |
| `SMB2 & SMB3 Servers` | `Not Required`              |
| `Domain Controllers`  | `Required`                  |

After Windows 11 insider editions, all behavior required SMB signing.

In the context of `NTLM`, `sealing` also implies `signing` because every `sealed` message is also `signed`.
### Hash Protocol Comparison

|**Hash/Protocol**|**Cryptographic technique**|**Mutual Authentication**|**Message Type**|**Trusted Third Party**|
|---|---|---|---|---|
|`NTLM`|Symmetric key cryptography|No|Random number|Domain Controller|
|`NTLMv1`|Symmetric key cryptography|No|MD4 hash, random number|Domain Controller|
|`NTLMv2`|Symmetric key cryptography|No|MD4 hash, random number|Domain Controller|
|`Kerberos`|Symmetric key cryptography & asymmetric cryptography|Yes|Encrypted ticket using DES, MD5|Domain Controller/Key Distribution Center (KDC)|



























