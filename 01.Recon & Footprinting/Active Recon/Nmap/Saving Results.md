
---
- Normal output (`-oN`) with the `.nmap` file extension
- Grepable output (`-oG`) with the `.gnmap` file extension
- XML output (`-oX`) with the `.xml` file extension

To convert the stored results from XML format to HTML, we can use the tool `xsltproc`.
```bash
xsltproc target.xml -o target.html
```
