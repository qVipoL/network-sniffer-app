#ifndef __SNIFFER_H
#define __SNIFFER_H

int create_raw_socket();
int sniff_packets(int raw_sd);

#endif