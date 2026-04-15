
---
When the plain text is encrypted and decrypted using the same key, it is known as symmetric encryption.t is also known as "shared-key" or "private-key" encryption. It ensures confidentiality by ensuring only authorized parties with the key can access the original data.

1. Key Generation :
	A secure key is generated using algorithms like PBKDF2 (Password-Based Key Derivation Function 2) or hardware random number generators. This key must be securely shared or transferred over the network for future use.
	
2. Encryption :
	The original message (plain text) is transformed into unreadable text (ciphertext) ,and the plaintext is processed in blocks or streams using an encryption algorithm and the secret key.
	
3. Transfer of Cipher text :
	The encrypted message (ciphertext) is then sent over the network. Even if intercepted, it remains unreadable to the attacker unless they have access to the shared secret key and the algorithm used for encryption.
	
4. Decrypyion :
	In the final step, the recipient uses the same secret key and a reverse encryption algorithm to convert the cipher text back into the original message (plain text).
