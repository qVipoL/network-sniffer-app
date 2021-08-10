#include "../include/printer.h"
#include "../include/sniffer.h"
#include "../include/std_include.h"

int main(int argc, char *argv[]) {
    int raw_sd, status_code, i = 0;
    char options[PRINTER_OPTIONS_MAX], option;

    if (argc < 2 || argc > PRINTER_OPTIONS_MAX + 1) {
        display_printer_options();
        return 1;
    }

    while ((option = getopt(argc, argv, PRINTER_OPTIONS)) != -1)
        options[i++] = option;

    if (i == 0) {
        display_printer_options();
        return 1;
    }

    options[i] = '\0';

    if ((raw_sd = create_raw_socket()) < 0) {
        perror("socket error");
        return 1;
    }

    if ((status_code = init_printer(options) < 0)) {
        display_printer_options();
        close(raw_sd);
        return 1;
    }

    if ((status_code = sniff_packets(raw_sd)) < 0) {
        perror("sniffing error");
        close(raw_sd);
        return 1;
    }

    close(raw_sd);
    return 0;
}