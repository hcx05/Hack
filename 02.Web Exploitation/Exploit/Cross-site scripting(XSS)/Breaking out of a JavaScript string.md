
---
In cases where the XSS context is inside a quoted string literal, it is often possible to break out of the string and execute JavaScript directly.

**`'-alert(document.domain)-'`**:

> "Break out of the string literal using the subtraction operator to concatenate the string with the function's execution result."

**`';alert(document.domain)//`**:

> "Terminate the current statement with a semicolon, execute the malicious script, and use comment characters to neutralize the remaining code."

