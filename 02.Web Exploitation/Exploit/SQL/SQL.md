
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
`select * from users where username != 'admin';`

| id  | username | password  |
| --- | -------- | --------- |
| 1   | jon      | pass123   |
| 3   | martin   | serect123 |
`select * from users where username='admin' or username='jon';`

| id  | username | password |
| --- | -------- | -------- |
| 1   | jon      | pass123  |
| 2   | admin    | p4ssword |
`select * from users where username='admin' and password='p4ssword';`

| id  | username | password |
| --- | -------- | -------- |
| 2   | admin    | p4ssword |
`select * from users where username like 'a%';`

| id  | username | password |
| --- | -------- | -------- |
| 2   | admin    | p4ssword |
`select * from users where username like '%n';`

| id  | username | password  |
| --- | -------- | --------- |
| 1   | jon      | pass123   |
| 2   | admin    | p4ssword  |
| 3   | martin   | secert123 |

This returns any rows with a username ending with the letter n.

`select * from users where username like '%mi%';`

| id  | username | password |
| --- | -------- | -------- |
| 2   | admin    | p4ssword |
**UNION**

| id  | name             | address         | city       | postcode |
| --- | ---------------- | --------------- | ---------- | -------- |
| 1   | Mr John Smith    | 123 Fake Street | Manchester | M2 3FJ   |
| 2   | Mrs Jenny Palmer | 99 Green Road   | Birmingham | B2 4KL   |
| 3   | Miss Sarah Lewis | 15 Fore Street  | London     | NW12 3GH |

| id  | company          | address                     | city    | postcode |
| --- | ---------------- | --------------------------- | ------- | -------- |
| 1   | Widgets Ltd      | Unit 1a, Newby Estate       | Bristol | BS19 4RT |
| 2   | The Tool Company | 75 Industrial Road          | Norwich | N22 3DR  |
| 3   | Axe Makers Ltd   | 2b Makers Unit, Market Road | London  | SE9 1KK  |

`SELECT name,address,city,postcode from customers UNION SELECT company,address,city,postcode from suppliers;`

| name             | address                     | city       | postcode |
| ---------------- | --------------------------- | ---------- | -------- |
| Mr John Smith    | 123 Fake Street             | Manchester | M2 3FJ   |
| Mrs Jenny Palmer | 99 Green Road               | Birmingham | B2 4KL   |
| Miss Sarah Lewis | 15 Fore Street              | London     | NW12 3GH |
| Widgets Ltd      | Unit 1a, Newby Estate       | Bristol    | BS19 4RT |
| The Tool Company | 75 Industrial Road          | Norwich    | N22 3DR  |
| Axe Makers Ltd   | 2b Makers Unit, Market Road | London     | SE9 1KK  |
**INSERT**

`insert into users (username,password) values ('bob','password123');`

| id  | username | password    |
| --- | -------- | ----------- |
| 1   | jon      | pass123     |
| 2   | admin    | p4ssword    |
| 3   | amrtin   | secert123   |
| 4   | bob      | password123 |
**UPDATE**

`update users SET username='root',password='pass123' where username='admin';`

| id  | username | password    |
| --- | -------- | ----------- |
| 1   | jon      | pass123     |
| 2   | root     | pass123     |
| 3   | martin   | serect123   |
| 4   | bob      | password123 |
**DELETE**

`delete from users where username='martin';`

| id  | username | password    |
| --- | -------- | ----------- |
| 1   | jon      | pass123     |
| 2   | root     | pass123     |
| 3   | bob      | password123 |

`delete from users;`

| id  | username | password |
| --- | -------- | -------- |
