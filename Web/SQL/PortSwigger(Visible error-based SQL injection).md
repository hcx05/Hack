
---
Firsrt, use subquery to check if the database is Oracle, it turns out that it isn't.
And notice this lab use character limit to prevent users from triggering conditional responses.

`' || cast((select password from users limit 1) as int)--`
