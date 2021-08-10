#ifndef __PRINTER_H
#define __PRINTER_H

#include "std_include.h"

#define PRINTER_OPTIONS_MAX 3
#define PRINTER_OPTIONS "itu"

void print_packet(char *buffer, size_t size);
int init_printer(char *options);
void display_printer_options();

#endif