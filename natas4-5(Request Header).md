
---
Web requests generally includes the request method (GET, POST,…), the requested URL and the protocol version. However, it can also include additional, potentially needed information, through so-called **request headers**.

```sql
curl -u {name:passward} {URL} -H "Header-Name: Header-Value"
// -i : response header
// -v : complete connection detail
// -I : desplay header only
```