#include "Arduino.h"
#include "Menu_Utilities.h"

int scrolling_timer;

void draw_options(TFT_eSPI * tft, char options[][MAX_OPTION_LEN1], int length, int selection, boolean highlight_selection){
  int starting_index = find_starting_index(tft,options,selection);
  for(int i = starting_index; i < length; i++){
    char temp[MAX_OPTION_LEN1+50];
    strcpy(temp,options[i]);
    fill_line(temp,tft->textWidth(temp),tft->getRotation()==1,tft->textWidth(" "));
    if(selection == i && highlight_selection) highlight(tft,temp);
    else tft->println(temp);
  }
  int h, w;
  if(!tft->getRotation()){
    h = SCREEN_HEIGHT;
    w = SCREEN_WIDTH;
  }
  else{
    w = SCREEN_HEIGHT;
    h = SCREEN_WIDTH;
  } 
  tft->fillRect(tft->getCursorX(),tft->getCursorY(),w,h-tft->getCursorY(),BACKGROUND);
}

void fill_line(char * str, uint16_t text_width, boolean rotation, uint16_t space_width){
  int h, w;
  if(!rotation){
    h = SCREEN_HEIGHT;
    w = SCREEN_WIDTH;
  }
  else{
    w = SCREEN_HEIGHT;
    h = SCREEN_WIDTH;
  } 
  int num_spaces = (w  - (text_width%(w-(w%space_width))))/space_width;
  while(num_spaces > 0){
    strcat(str," ");
    num_spaces--;
  }
}

int find_starting_index(TFT_eSPI * tft, char options[][MAX_OPTION_LEN1], int selection){
  int w,h;
  int font_height = tft->fontHeight();
  if(tft->getRotation() == 0){
    h = SCREEN_HEIGHT;
    w = SCREEN_WIDTH;
  }
  else{
    w = SCREEN_HEIGHT;
    h = SCREEN_WIDTH;
  } 
  
  int cursor_pos = h - ceil(1.0*tft->textWidth(options[selection])/w)*font_height;
  int index = selection;
  
  while(true){
    if(index == 0)
      break;
    cursor_pos -= ceil(1.0*tft->textWidth(options[index-1])/w)*font_height;
    if(!(cursor_pos >= tft->getCursorY()))
      break;
    index--;
  }
  return index;
}

void draw_text(TFT_eSPI * tft, char * text, char options[][MAX_OPTION_LEN1], int num_options, int selection){
  tft->println(text);
  draw_options(tft,options,num_options,selection);
}

void highlight(TFT_eSPI * tft, char * text){
  tft->setTextColor(TFT_BLACK,HIGHLIGHT_COLOR);
  tft->println(text);
  tft->setTextColor(TEXT_COLOR,BACKGROUND);
}

void change_selection(int * current_selection, int selection_length){
  *current_selection += 1;
  *current_selection %= selection_length;
}

void update_selection(IMU * imu,int * current_selection,int num_selections) {
  float x, y;
  imu->get_angle(&x, &y);
  if(millis()-scrolling_timer >= scrolling_threshold){
      if(-y > angle_threshold && *current_selection < num_selections-1){
        *current_selection+=1;
        *current_selection %= num_selections;
        scrolling_timer = millis();
      }
      else if(-y < -angle_threshold && *current_selection > 0){
        *current_selection-=1;
        *current_selection += num_selections;
        *current_selection %= num_selections;
        scrolling_timer = millis();
      }
    }
 }
