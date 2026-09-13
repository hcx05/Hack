
---
```bash
curl -s "https://crt.sh/?q={Target_URL}&output=json" | jq -r '.[] | select(.name_value | contains("dev")) | .name_value' | sort -u
```