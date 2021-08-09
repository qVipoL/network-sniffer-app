#include "../include/sniffer.h"
#include "../include/std_include.h"

int main() {
    int raw_sd, status_code;

    if ((raw_sd = create_raw_socket()) < 0) {
        perror("socket error");
        return 1;
    }

    if ((status_code = sniff_packets(raw_sd)) < 0) {
        perror("sniffing error");
        return 1;
    }

    return 0;
}