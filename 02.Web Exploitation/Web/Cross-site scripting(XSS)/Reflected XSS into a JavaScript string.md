
---
First try :

```html
</script><img src=javascript:alert()>
```

Didn't work, because in the modern browsers, <img> `src` attribute in the `img` tag is to load pictures, when the browser attempt to get this Pseudo-Protocol, it fail due to the reason that this URL is not a proper image format.

`javascript:` in `src` won't execute code automatically like `href` in `<a>` did.

So try :

```html
</script><img src=1 onerror=alert()>
```

