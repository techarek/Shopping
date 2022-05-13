#ifndef Wifi_Utilities_h
#define Wifi_Utilities_h
#include "Arduino.h"
#include <WiFi.h>
const int RESPONSE_TIMEOUT = 6000;
void connect_wifi(char * network, char* password);
uint8_t char_append(char* buff, char c, uint16_t buff_size);
void do_http_request(char* host, char* request, char* response, uint16_t response_size, uint16_t response_timeout=RESPONSE_TIMEOUT, uint8_t serial=1);
#endif
