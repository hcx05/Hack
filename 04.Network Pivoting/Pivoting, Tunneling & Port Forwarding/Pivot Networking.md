
---
Lateral Movement helps us spread wide within a network, elevating our privileges, while Pivoting allows us to delve deeper into the networks accessing previously unreachable environments. Keep this comparison in mind while moving through this module.

### IP Addressing & NICs

---
The IP address is assigned in software and usually obtained automatically from a DHCP server.

The IP address is assigned to a `Network Interface Controller` (`NIC`).Commonly, the NIC is referred to as a `Network Interface Card` or `Network Adapter`.

Every IPv4 address will have a corresponding `subnet mask`. The subnet mask defines the `network` & `host` portion of an IP address.

### Routing

---
It is common to think of a network appliance that connects us to the Internet when thinking about a router, but technically any computer can become a router and participate in routing. 

One key defining characteristic of a router is that it has a routing table that it uses to forward traffic based on the destination IP address.

```bash
❯ netstat -r

Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
default         192.168.3.2     0.0.0.0         UG        0 0          0 eth0
10.10.8.0       10.10.14.1      255.255.252.0   UG        0 0          0 tun0
10.10.14.0      0.0.0.0         255.255.254.0   U         0 0          0 tun0
10.10.110.0     10.10.14.1      255.255.255.0   UG        0 0          0 tun0
10.13.37.0      10.10.14.1      255.255.255.0   UG        0 0          0 tun0
10.13.38.0      10.10.14.1      255.255.255.0   UG        0 0          0 tun0
10.129.0.0      10.10.14.1      255.255.0.0     UG        0 0          0 tun0
192.168.3.0     0.0.0.0         255.255.255.0   U         0 0          0 eth0
192.168.52.0    0.0.0.0         255.255.255.0   U         0 0          0 eth1
```

Or using a more modern way
```bash
❯ ip route

default via 192.168.3.2 dev eth0 proto dhcp src 192.168.3.128 metric 100
10.10.8.0/22 via 10.10.14.1 dev tun0
10.10.14.0/23 dev tun0 proto kernel scope link src 10.10.15.138
10.10.110.0/24 via 10.10.14.1 dev tun0
10.13.37.0/24 via 10.10.14.1 dev tun0
10.13.38.0/24 via 10.10.14.1 dev tun0
10.129.0.0/16 via 10.10.14.1 dev tun0
192.168.3.0/24 dev eth0 proto kernel scope link src 192.168.3.128 metric 100
192.168.52.0/24 dev eth1 proto kernel scope link src 192.168.52.128 metric 101
```
Any traffic destined for networks not present in the routing table will be sent to the default route, which can also be referred to as the default gateway or gateway of last resort.















