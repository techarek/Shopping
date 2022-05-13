#include "Arduino.h"
#include "Button.h"

Button::Button(int p) {
    flag = 0;  
    state = 0;
    pin = p;
    pinMode(pin,INPUT_PULLUP);
    t_of_state_2 = millis(); //init
    t_of_button_change = millis(); //init
    debounce_time = 10;
    long_press_time = 1000;
    button_pressed = 0;
  }
  
void Button::read() {
  uint8_t button_state = digitalRead(pin);  
  button_pressed = !button_state;
}

int Button::update() {
  read();
  flag = 0;
  switch(state){
    case(0):
      if(button_pressed){
        state = 1;
        t_of_button_change = millis();
      }
      break;
    case(1):
      if(!button_pressed) state = 0;
      else if(millis() - t_of_button_change >= debounce_time){
        state = 2;
        t_of_state_2 = millis();
      }
      break;
    case(2):
      if(!button_pressed){
        state = 4;
        t_of_button_change = millis();
      }
      else if(millis() - t_of_state_2 >= long_press_time) state = 3;
      break;
    case(3):
      if(!button_pressed){
        state = 4;
        t_of_button_change = millis();
      }
      break;
    case(4):
      if(button_pressed){
        if(millis() - t_of_state_2 >= long_press_time) state = 3;
        else state = 2;
      }
      else if(millis()-t_of_button_change >= debounce_time){
        state = 0;
        if(t_of_button_change - t_of_state_2 >= long_press_time) flag = 2;
        else flag = 1;
      }
      break;
  }
  return flag;
}
