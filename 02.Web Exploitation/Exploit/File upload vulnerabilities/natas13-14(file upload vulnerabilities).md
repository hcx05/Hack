
---
```php
else if (! exif_imagetype($_FILES['uploadedfile']['tmp_name'])) {
        echo "File is not an image";
// exif_imagetype will check Magic Bits at the beginning of the file
```

add `GIF89a` to our file (Magic bits of GIF)