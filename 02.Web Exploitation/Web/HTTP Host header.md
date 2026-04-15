
---
HTTP  Host header specifies the domain name that client wants to access. In some cases, such as when the request has been forwarded by an intermediary system, the Host value may be altered before it reaches the intended back-end component.

```
Host: portswigger.net
```

Purpose : Help identify which back-end component the client wants to communicate with.

Virtual hosting : When a single web server hosts multiple websites or applications. This could be multiple websites with a single owner, but it is also possible for websites with different owners to be hosted on a single, shared platform.