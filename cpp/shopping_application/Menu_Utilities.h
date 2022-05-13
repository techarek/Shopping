#ifndef Menu_Utilities_h
#define Menu_Utilities_h
#include "Arduino.h"
#include <string.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "IMU.h"
#include <cmath>

#define BACKGROUND TFT_BLACK
#define TEXT_COLOR TFT_GREEN
#define HIGHLIGHT_COLOR TFT_WHITE

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT  = 160;
const int scrolling_threshold = 250;
const float angle_threshold = 0.3;
const int MAX_OPTION_LEN1 = 100;
void draw_options(TFT_eSPI * tft, char options[][MAX_OPTION_LEN1], int length, int selection, boolean highlight_selection = true);
int find_starting_index(TFT_eSPI * tft, char options[][MAX_OPTION_LEN1], int selection);
void draw_text(TFT_eSPI * tft, char * text, char options[][MAX_OPTION_LEN1], int num_options, int selection);
void highlight(TFT_eSPI * tft, char * text);
void change_selection(int * current_selection, int selection_length);
void update_selection(IMU * imu,int * current_selection,int num_selections);
void fill_line(char * str, uint16_t text_width, boolean rotation, uint16_t space_width);
#endif
