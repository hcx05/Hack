
---
Content type `application/x-www-form-urlencoded` is for sending simple text like names or addresses, while `multipart/form-data` is for sending large amounts of binary data like entire image file or a PDF document.

One way that websites may attempt to validate file uploads is to check that this input-specific `Content-Type` header matches an expected MIME type. If the server is only expecting image files, for example, it may only allow types like `image/jpeg` and `image/png`. Problems can arise when the value of this header is implicitly trusted by the server.