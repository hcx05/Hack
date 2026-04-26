
---
-sS: The TCP-SYN scansends one packet with the SYN flag and, therefore, never completes the three-way handshake, which results in not establishing a full TCP connection to the scanned port.
	- If our target sends a `SYN-ACK` flagged packet back to us, Nmap detects that the port is `open`.
	- If the target responds with an `RST`(Reset) flagged packet, it is an indicator that the port is `closed`.
	- If Nmap does not receive a packet back, it will display it as `filtered`. Depending on the firewall configuration, certain packets may be dropped or ignored by the firewall.