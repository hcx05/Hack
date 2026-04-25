
---
Some applications only allow inputs that match, a whitelist of permitted values. The filter may look for a match at the beginning of the input, or contained within in it. You may be able to bypass this filter by exploiting inconsistencies in URL parsing.

The URL specification contains a number of features that are likely to be overlooked when URLs implement ad-hoc parsing and validation using this method:

- Embed credentials in a URL before the hostname, using the `@` character

```
	https://expected-host:fakepassword@evil-host
```
- Use the `#` character to indicate a URL fragment.

```
	https://evil-host#expected-host
```
- Leverage the DNS naming hierarchy to place required input into a fully-qualified DNS name that you control.
```
	https://expected-host.evil-host
```
- URL-encode characters to confuse the URL-parsing code. This is particularly useful if the code that implements the filter handles URL-encoded characters differently than the code that performs the back-end HTTP request. You can also try ***double-encoding*** characters; some servers recursively URL-decode the input they receive, which can lead to further discrepancies.

