
---
SQL (Structured Query Language) is a feature-rich language used for querying databases. These SQL queries are better referred to as statements.

SQL syntax is not case-sensitive.

`select * from users;`

| id  | username | password  |
| --- | -------- | --------- |
| 1   | jon      | pass123   |
| 2   | admin    | p4ssword  |
| 3   | martin   | serect123 |
The first word SELECT, tells the database we want to retrieve some data; the * tells the database we want to receive back all columns from the table.

`select username,password from users;`

| username | password  |
| -------- | --------- |
| jon      | pass123   |
| admin    | p4ssword  |
| martin   | secret123 |
