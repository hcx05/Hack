
---
Asymmetric encryption, also known as public-key cryptography, is a type of encryption that uses a pair of keys to encrypt and decrypt data. The pair of keys includes a public key, which can be shared with anyone, and a private key, which is kept secret by the owner.

In asymmetric encryption, the sender uses the recipient's public key to encrypt the data. The recipient then uses their private key to decrypt the data. This approach allows for secure communication between two parties without the need for both parties to have the same secret key.

The Main Features of Asymmetric Encryption (also known as public-key cryptography) :

Dual keys, 

Encryption and decryption : 
	Asymmetric encryption uses the public key to encrypt data and the private key to decrypt data. This allows secure communication between two parties without the need to exchange secret keys.

DIgital signatures :
	Asymmetric encryption enables the creation of digital signatures, which can be used to verify the authenticity of data. A digital signature is created by encrypting a hash of the data with the ***sender***'s private key.

Secure key exchange :
	Asymmetric encryption allows for secure key exchange, which is a critical feature in secure communication. For example, the ***Diffie-Hellman key exchange algorithm*** uses asymmetric encryption to establish a shared secret key between two parties without exchanging the key itself.


1. Key Pair Generation

2. Encryption :
	- The sender uses the public key of the recipient to encrypt the message.
	- The public key is used because it is freely available and can be used to convert the message into an unreadable format (cipher text).

3. Transmission :
	- The encrypted message (cipher text) is sent over the network to the recipient. Even if intercepted by someone, it remains unreadable without the corresponding private key.
	
4. Decryption :
	- Upon receiving the encrypted message, the recipient uses their private key to decrypt it.
	- The private key is kept secure and is never shared, ensuring that only the intended recipient can decrypt and read the message.
	
5. Verification (Digital Sighature)
	- In some cases, the sender may also sign the message using their private key to ensure its authenticity.
	- The recipient can verify the signature using the ***sender***'s public key, confirming that the message was not tampered with and was indeed sent by the intended sender.

