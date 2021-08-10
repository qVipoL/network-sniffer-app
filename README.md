## Packet Sniffer

### How To Use:
* Compile (production): <code>make release</code>
* Sniffing Options (can be combined):
1) -i -> ICMP packets
2) -t -> TCP packets
3) -u -> UDP packets
* Run: <code>sudo ./bin/sniffer-app -i -t -u</code>