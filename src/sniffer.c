#include "../include/sniffer.h"

#include "../include/printer.h"
#include "../include/std_include.h"

#define BUFFER_SIZE 256

static volatile bool alive = true;

static void process_packet(char *data, size_t data_size);

int create_raw_socket() {
    int raw_sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (raw_sd < 0)
        return -1;

    return raw_sd;
}

int sniff_packets(int raw_sd) {
    size_t saddr_size, data_size;
    struct sockaddr saddr;
    char buffer[BUFFER_SIZE];

    printf("Sniffing Packets...\n");

    while (alive) {
        data_size = recvfrom(raw_sd, buffer, BUFFER_SIZE, 0, &saddr, (socklen_t *)&saddr_size);

        if (data_size < 0) {
            alive = false;
            return -1;
        }

        process_packet(buffer, data_size);
    }

    return 0;
}

static void process_packet(char *data, size_t data_size) {
    print_packet(data, data_size);
}