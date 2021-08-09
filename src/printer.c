#include "../include/printer.h"

#include "../include/std_include.h"

#define MAX_PROTOCOLS 7

static void print_ethernet_header(char *buffer, size_t size);
static void print_ip_header(char *buffer, size_t size);
static void print_hex_data(char *buffer, size_t size);
static void print_tcp_packet(char *buffer, size_t size);

static void (*protocol_to_print[MAX_PROTOCOLS])(char *buffer, size_t size) = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    print_tcp_packet  // 6 - TCP protocol
};

void print_packet(char *buffer, size_t size) {
    struct iphdr *ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    int protocol = ip_header->protocol;

    if (protocol < MAX_PROTOCOLS && protocol_to_print[protocol] != NULL)
        protocol_to_print[ip_header->protocol](buffer, size);
    else
        printf("protocol not supported.\n");
}

static void print_ethernet_header(char *buffer, size_t size) {
    struct ethhdr *ethernet_header = (struct ethhdr *)buffer;

    printf("Ethernet Header\n");
    printf("\t|-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", ethernet_header->h_source[0], ethernet_header->h_source[1], ethernet_header->h_source[2], ethernet_header->h_source[3], ethernet_header->h_source[4], ethernet_header->h_source[5]);
    printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", ethernet_header->h_dest[0], ethernet_header->h_dest[1], ethernet_header->h_dest[2], ethernet_header->h_dest[3], ethernet_header->h_dest[4], ethernet_header->h_dest[5]);
    printf("\t|-Protocol            : %u\n", (int)ethernet_header->h_proto);
}

static void print_ip_header(char *buffer, size_t size) {
    struct sockaddr_in source, dest;
    struct iphdr *ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    print_ethernet_header(buffer, size);

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip_header->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip_header->daddr;

    printf("\nIP Header\n");
    printf("\t|-Version             : %d\n", (int)ip_header->version);
    printf("\t|-Inter Header Length : %d DWORDS or %d Bytes\n", (int)ip_header->ihl, ((int)(ip_header->ihl)) * 4);
    printf("\t|-Type Of Service     : %d\n", (int)ip_header->tos);
    printf("\t|-Total Length        : %d Bytes\n", ntohs(ip_header->tot_len));
    printf("\t|-Identification      : %d\n", ntohs(ip_header->id));
    printf("\t|-Time To Live        : %d\n", (int)ip_header->ttl);
    printf("\t|-Protocol            : %d\n", (int)ip_header->protocol);
    printf("\t|-Header Checksum     : %d\n", ntohs(ip_header->check));
    printf("\t|-Source IP           : %s\n", inet_ntoa(source.sin_addr));
    printf("\t|-Destination IP      : %s\n", inet_ntoa(dest.sin_addr));
}

static void print_hex_data(char *buffer, size_t size) {
    int i;

    for (i = 0; i < size; i++) {
        if (i != 0 && i % 16 == 0)
            printf("\n");

        printf("  %02X", (int)buffer[i] & 0xff);
    }
}

static void print_tcp_packet(char *buffer, size_t size) {
    int ip_header_size, tcp_header_size;
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;

    ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    ip_header_size = ip_header->ihl * 4;
    tcp_header = (struct tcphdr *)(buffer + ip_header_size + sizeof(struct ethhdr));
    tcp_header_size = sizeof(struct ethhdr) + ip_header_size + tcp_header->doff * 4;

    printf("\n***********************TCP Packet*************************\n");

    print_ip_header(buffer, size);

    printf("\nTCP Header\n");
    printf("\t|-Source Port        : %u\n", ntohs(tcp_header->source));
    printf("\t|-Destination Port   : %u\n", ntohs(tcp_header->dest));
    printf("\t|-Sequence Number    : %u\n", ntohl(tcp_header->seq));
    printf("\t|-Acknowledge Number : %u\n", ntohl(tcp_header->ack_seq));
    printf("\t|-Header Length      : %d DWORDS or %d BYTES\n", (int)tcp_header->doff, (int)tcp_header->doff * 4);
    printf("\t|------------FLAGS------------\n");
    printf("\t\t|-Urgent Flag          : %d\n", (int)tcp_header->urg);
    printf("\t\t|-Acknowledgement Flag : %d\n", (int)tcp_header->ack);
    printf("\t\t|-Push Flag            : %d\n", (int)tcp_header->psh);
    printf("\t\t|-Reset Flag           : %d\n", (int)tcp_header->rst);
    printf("\t\t|-Synchronise Flag     : %d\n", (int)tcp_header->syn);
    printf("\t\t|-Finish Flag          : %d\n", (int)tcp_header->fin);
    printf("\t|-Window Size       : %d\n", ntohs(tcp_header->window));
    printf("\t|-Checksum          : %d\n", ntohs(tcp_header->check));
    printf("\t|-Urgent Pointer    : %d\n", tcp_header->urg_ptr);
    printf("\nData\n");
    print_hex_data(buffer + tcp_header_size, size - tcp_header_size);

    printf("\n**********************************************************\n");
}
