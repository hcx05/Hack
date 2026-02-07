
---
```jsx
nmap -p- -sV -Pn {target_IP} // -p- : scan all the ports
```

Redis : Remote Dictionary Server, an ‘in-memory’ database. In-memory databases are the ones that rely essentially on the primary memory for data storage (meaning that the database is managed in the RAM of the system)

```jsx
redis- cli --help
```

```jsx
redis-cli -h {target_IP}
```

```jsx
redis-cli info
```

```jsx
select 0 // select the database with index 0
```

```jsx
keys * // list all the keys in the database
```