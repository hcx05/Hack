
---
SQL (Structured Query Language) is a feature-rich language used for querying databases. These SQL queries are better referred to as statements.

SQL syntax is not case-sensitive.

`select * from users;`

| id  | username | password  |
| --- | -------- | --------- |
| 1   | jon      | pass123   |
| 2   | admin    | p4ssword  |
| 3   | martin   | serect123 |
The first word SELECT, tells the database we want to retrieve some data; the * tells the database we want to receive back all ***columns*** from the table.

`select username,password from users;`

| username | password  |
| -------- | --------- |
| jon      | pass123   |
| admin    | p4ssword  |
| martin   | secret123 |
`select * from users LIMIT 1;`

| id  | username | password |
| --- | -------- | -------- |
| 1   | jon      | pass123  |
The following query, like the first, returns all the columns by using the * selector, and then the "`LIMIT 1`" clause forces the database to return only one row of data. Changing the query to 
"`LIMIT 1,1`" forces the query to skip the first result, and then "`LIMIT 2,1`" skips the first two results, and so on.The first number tells the database how many results you wish to skip, and the second number tells the database how many rows to return.

`select * from users where username='admin';`

| id  | username | password |
| --- | -------- | -------- |
| 2   | admin    | pass123  |
