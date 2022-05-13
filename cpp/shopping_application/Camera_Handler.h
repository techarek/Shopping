#ifndef Camera_Handler_h
#define Camera_Handler_h

#include "Arduino.h"
#include "Wifi_Utilities.h"
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "memorysaver.h"

const int CAMERA_RESPONSE_TIMEOUT = 10000; //ms to wait for response from host

static const size_t camera_buffer_size = 600;

const uint16_t CAMERA_IN_BUFFER_SIZE = 4*camera_buffer_size + 500; //size of buffer to hold HTTP request
const uint16_t CAMERA_OUT_BUFFER_SIZE = 4*camera_buffer_size + 500; //size of buffer to hold HTTP response

// set GPIO17 as the slave select :
const int CS = 17;
const int CAM_POWER_ON = 10;

const long BITMAP_SIZE = 19200;

void init_camera(ArduCAM * myCAM, int cs = CS, int cam_power_on = CAM_POWER_ON);
void start_capture(ArduCAM * myCAM);
void camCapture(ArduCAM * myCAM);
void ints_to_string(int* input, int input_length, char * output);
void camDisplay(ArduCAM * myCAM, TFT_eSPI * tft,uint16_t * bitmap);
void send_buffer(char * str_buffer, bool new_photo = false);
void take_photo(ArduCAM * myCAM);
void show_image(ArduCAM * myCAM, TFT_eSPI * tft, uint16_t * bitmap);
void change_format(ArduCAM * myCAM, byte fmt);
#endif
