
---
[[../../Web/SQL/SQL]] : Structured Query Language

PII : Personally Identifiable Information

```jsx
nmap -sV -sC -Pn {traget_IP} 
//-sC : Performs a script scan using the default set of scripts.
```

SQL Injection : A common way of exploiting web pages that use SQL Statements that retrieve and store user input data.

443 TCP : Standard port for https.

80 TCP : Standard port for http.

http : Hypertext Transfer Protocol, an application-layer protocol used for transmitting hypermedia documents, such as HTML (Hypertext Markup Language).

Before SQL injection, we check if there are any other directories or pages useful for us in the enumeration process.

```jsx
HTTP1/1 200 OK : Page/resource exists, proceeds with sending you the data.
HTTP1/1 404 Not Found : Page/resource does not exist.
HTTP1/1 302 Found : Page/resource found, but by redirection to another directory (moved
temporarily). This is an invitation to the user-agent (the web browser) to make a second,
identical request to the new URL specified in the location field. You will perceive the
whole process as a seamless redirection to the new URL of the specified resource.
```

```bash
gobuster --help
```

```jsx
gobuster dir -u http://{target_IP} -w {wordlist_location}/directory-list-2.3-small.txt
```

Nothing useful found, do SQL injection :

```sql
$sql="SELECT * FROM users WHERE username='$username' AND password='$password'"; 
```

If we let $username=name’ #, the statement become :