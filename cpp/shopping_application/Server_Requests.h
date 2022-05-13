#ifndef Server_Requests_h
#define Server_Requests_h
#include "Arduino.h"
#include <string.h>
#include "Wifi_Utilities.h"
#include "GPS.h"
const uint16_t IN_BUFFER_SIZE = 1000; //size of buffer to hold HTTP request
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP response
const int MAX_OPTION_LEN = 100;
void add_item(char * username, char * item, int selection, char * UPC, GPS * gps);
void get_items(char list[][MAX_OPTION_LEN], char UPC_list[][15], int * list_counts, int list_len, char * username);
void get_stores(GPS * gps, char * stores);
void get_product_description(char * output, char * UPC);
#endif
