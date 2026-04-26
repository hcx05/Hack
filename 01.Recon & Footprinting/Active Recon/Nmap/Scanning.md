
---
### Host scanning
- -sn: 
	No Port scanning
- -PE(ICMP Echo Request):
	- Standerd `ping`, Sent Type 8(Echo Request), hoping for Type 0(Echo Reply).
	- Often been block by Firewall.
- -PP(ICMP Timestamp Request):
	- Sent Type 13(Time stamp Request), hoping for Type 14(System time reply), then we can confirmed that the host was alive indirectly.
- -PM(ICMP Address Mask Request): 
	- Sent Type 17(Subnet masking Request), most of the system, usually Linux, won't reply this request for security reason. But in some old or misconfigurated system, it can bypass the Firewall.
