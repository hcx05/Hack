
---
The application whose URLs are allowed contains an open redirection vulnerability. Provided the API used to make the back-end HTTP request supports redirections, you can construct a URL that satisfies the filter and results in a redirected request to the desired back-end target.

First, find the proper open redirection, then leverage the open redirection vulnerability to bypass the URL filter, and exploit the SSRF vulnerability.