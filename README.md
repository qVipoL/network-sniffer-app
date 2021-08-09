## Packet Sniffer

### How To Use:
* Compile (production): <code>make release</code>
* Run: <code>sudo ./bin/sniffer-app</code>

### Improvements:
* Additional functionality can be added to:
1) src/printer.c - add function pointers to print other protocols
```c
// resloves protocol number to a printing function
static void (*protocol_to_print[MAX_PROTOCOLS])(char *buffer, size_t size); 
```
2) src/sniffer.c - add additional packet processing
```c
static void process_packet(char *data, size_t data_size) {
    /* processing logic here */
}
```