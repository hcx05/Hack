
---
A comprehensive list of archive file types can be found on [FileInfo](https://fileinfo.com/filetypes/compressed).
```bash
hcx05@htb[/htb]$ curl -s https://fileinfo.com/filetypes/compressed | html2text | awk '{print tolower($1)}' | grep "\." | tee -a compressed_ext.txt

.mint
.zhelp
.b6z
.fzpz
.zst
.apz
.ufs.uzip
.vrpackage
.sfg
.gzip
.xapk
.rar
.pkg.tar.xz
<SNIP>

#Note that not all archive types support native password protection, and in such cases, additional tools are often used to encrypt the files. For example, TAR files are commonly encrypted using openssl or gpg.
```
### Cracking OpenSSL encrypted GZIP files
```bash
hcx05@htb[/htb]$ file GZIP.gzip 

GZIP.gzip: openssl enc'd data with salted password
```

```bash
hcx05@htb[/htb]$ for i in $(cat rockyou.txt);do openssl enc -aes-256-cbc -d -in GZIP.gzip -k $i 2>/dev/null| tar xz;done
#If its not archived, replace tar to gunzip or gzip -d

gzip: stdin: not in gzip format
tar: Child returned status 1
tar: Error is not recoverable: exiting now

gzip: stdin: not in gzip format
tar: Child returned status 1
tar: Error is not recoverable: exiting now
<SNIP>
```
Once the `for` loop has finished
```bash
hcx05@htb[/htb]$ ls

customers.csv  GZIP.gzip 
```
### Cracking BitLocker-encrypted drives
[BitLocker](https://docs.microsoft.com/en-us/windows/security/information-protection/bitlocker/bitlocker-device-encryption-overview-windows-10) is a full-disk encryption feature developed by Microsoft for the Windows operating system. Available since Windows Vista, it uses the `AES` encryption algorithm with either 128-bit or 256-bit key lengths. If the password or PIN used for BitLocker is forgotten, decryption can still be performed using a recovery key—a 48-digit string generated during the setup process.

To crack a BitLocker encrypted drive, we can use a script called `bitlocker2john` to [four different hashes](https://openwall.info/wiki/john/OpenCL-BitLocker)
```bash
hcx05@htb[/htb]$ bitlocker2john -i Backup.vhd > backup.hashes
hcx05@htb[/htb]$ grep "bitlocker\$0" backup.hashes > backup.hash
hcx05@htb[/htb]$ cat backup.hash

$bitlocker$0$16$02b329c0453b9273f2fc1b927443b5fe$1048576$12$00b0a67f961dd80103000000$60$d59f37e70696f7eab6b8f95ae93bd53f3f7067d5e33c0394b3d8e2d1fdb885cb86c1b978f6cc12ed26de0889cd2196b0510bbcd2a8c89187ba8ec54f
```
#### Browsing encrypted .vhd files on Linux
```bash
sudo apt update
sudo apt install dislocker
```

```bash
sudo mkdir -p /media/bitlocker
#bitlocker-file: Place the decrypted files.
sudo mkdir -p /media/bitlockermount
#bitlockermount: Place to browse the mounted files.
```
Use `losetup` to configure the VHD as [loop device](https://en.wikipedia.org/wiki/Loop_device)
```bash
sudo losetup -f -P Backup.vhd
#-f: Find an avalable /dev/loopX
#-P: Scan the partition of the virtual disk.
```
Decrypt the drive
```bash
sudo dislocker /dev/loop0p2 -u1234qwer -- /media/bitlocker
#-u: Passowrd
#--: End of the dislocker switches
```
Mount the decrypted volume
```bash
sudo mount -o loop /media/bitlocker/dislocker-file /media/bitlockermount
```





























