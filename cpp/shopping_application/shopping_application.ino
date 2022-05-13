/*----------------------INSTRUCTIONS FOR USE---------------------------  
 * Preliminaries
 *    - Make sure User_Setup.h in TFT library has variable SPI_Frequency 
 *      set to 10 Hz
 *    - Make sure you have buttons connected to pins 5 and 16
 *    - Make sure you have TFT, IMU, GPS, and ArduCAM connected
 * Usage
 *    - To select an item, press and release the button connected 
 *      to pin 16
 *    - To change your selection, tilt the device
 *    - To return to main menu at any time, press and release the
 *      button connected to pin 5
*/

#include "Wifi_Utilities.h"
#include "Camera_Handler.h"
#include "Button.h"
#include "Server_Requests.h"
#include <ArduCAM.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "memorysaver.h"
#include "GPS.h"
#include "IMU.h"
#include "Menu_Utilities.h"
#include <mpu6050_esp32.h>
#include <esp_task_wdt.h>

char network[] = "Jiwani";
char password[] = "7709123310";

uint16_t bitmap[BITMAP_SIZE];

ArduCAM myCAM(OV2640, CS);

TFT_eSPI tft = TFT_eSPI(); 

MPU6050 imu; //imu object
IMU my_imu = IMU(&imu);

HardwareSerial gps(2);
GPS my_gps = GPS(&gps);

const uint8_t BUTTON_PIN_1 = 16;
const uint8_t BUTTON_PIN_2 = 5;
Button button1(BUTTON_PIN_1);
Button button2(BUTTON_PIN_2);

const int MAX_LIST_LEN = 50;
//const int MAX_OPTION_LEN = 100;
const int MAX_ITEM_QUANTITY = 15;
const int SUCCESS_SCREEN_DURATION = 3000;

int state_timer;

enum State{Menu, AddItem, EditList, ConfirmImage, ProcessImage, ItemQuantity, Success, Item, FindStores};

State state;
State display_state;
State prev_state;

char MENU_TEXT[20] = "MAIN MENU\n";
char MENU_OPTIONS[3][MAX_OPTION_LEN] = {"Add Item","Edit List","Find Stores"};
State MENU_OPTION_STATES[3] = {AddItem, EditList, FindStores};

char CONFIRM_OPTIONS_TEXT[50] = "Is this your item:\n";
char CONFIRM_OPTIONS[2][MAX_OPTION_LEN] = {"Yes","Retake Photo"};
State CONFIRM_OPTION_STATES[2] = {ItemQuantity,AddItem};

char ITEM_QUANTITY_TEXT[20] = "How Many?\n";
char EDIT_LIST_TEXT[20] = "LIST";
char ITEM_TEXT[100] = "SELECT NEW QUANTITY (0 FOR REMOVE)\n ITEM: ";
char FIND_STORES_TEXT[20] = "STORES";

char success_message[150];

int selection;
char selection_str[5];
int prev_selection;

const int MAX_ITEM_STR_LEN = MAX_OPTION_LEN;
char item[MAX_ITEM_STR_LEN];
char UPC[15];
char list[MAX_LIST_LEN][MAX_ITEM_STR_LEN];
char UPC_list[MAX_LIST_LEN][15];
int list_counts[MAX_LIST_LEN];
int item_index;

const int MAX_STORE_DESCRIPTION_LEN = 500;
char stores[MAX_STORE_DESCRIPTION_LEN];

char username[10] = "USER4";

bool change_display = false;
bool begin_take_photo = true;
TaskHandle_t Task1;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  connect_wifi(network,password);
  init_camera(&myCAM);
  clear_strings();
  
  if (imu.setupIMU(1)) {
    Serial.println("IMU Connected!");
  } else {
    Serial.println("IMU Not Connected :/");
    Serial.println("Restarting");
    ESP.restart(); // restart the ESP (proper way)
  }
  //xTaskCreatePinnedToCore(Task1Code, "Task1", 10000, NULL,0, &Task1, 0); 
 }

void clear_strings(){
  strcpy(stores,"");
  strcpy(item,"");
  strcpy(selection_str,"");
  strcpy(success_message,"");
  for(int i = 0; i < MAX_LIST_LEN; i++){
    strcpy(list[i],"");
  }
}

void Task1Code(void * parameters){
  while(true){
  //Serial.println("TASK1");
  update_state(button1.update(),button2.update());
  //esp_task_wdt_feed();
  //if(state == AddItem || state == ProcessImage)
  vTaskDelay(20);
  }
}

void loop() {
  //Serial.println("TASK0");
  my_gps.extractGNRMC();
  update_state(button1.update(),button2.update());
  draw_screen();
}

void draw_screen(){
  tft.setCursor(0,0);
  if(change_display){
    change_display = false;
    tft.fillScreen(BACKGROUND);
  }
  int j = 0;
  switch(display_state){
    case Menu:
      draw_text(&tft,MENU_TEXT,MENU_OPTIONS,sizeof(MENU_OPTIONS)/sizeof(MENU_OPTIONS[0]),selection);
      break;
    case AddItem:
      xTaskCreatePinnedToCore(Task1Code, "Task1", 10000, NULL,0, &Task1, 0); 
      show_image(&myCAM,&tft,bitmap);
      vTaskDelete(Task1);
      break;
    case ProcessImage:
      highlight(&tft,"PROCESSING IMAGE... PLEASE WAIT");
      if(begin_take_photo){
        take_photo(&myCAM);
        get_product_description(item,UPC);
        begin_take_photo = false;
      }
      break;
    case ConfirmImage:
      char confirmation[MAX_ITEM_STR_LEN + 50];
      sprintf(confirmation,"%s%s",CONFIRM_OPTIONS_TEXT,item); 
      draw_text(&tft,confirmation,CONFIRM_OPTIONS,sizeof(CONFIRM_OPTIONS)/sizeof(CONFIRM_OPTIONS[0]),selection);
      break;
    case ItemQuantity:
      sprintf(selection_str,"%i",selection); 
      tft.println(ITEM_QUANTITY_TEXT);
      tft.println(selection_str);
      break;
    case Success:
      tft.println(success_message);
      break;
    case EditList:
      char list_with_cnts[MAX_LIST_LEN][MAX_ITEM_STR_LEN];
      for(int i = 0; i < MAX_LIST_LEN; i++){
        strcpy(list_with_cnts[i],"\0");
      }
      char tmp[MAX_ITEM_STR_LEN];
      while(strcmp(list[j],"")!=0 && j < MAX_LIST_LEN){
        sprintf(tmp,"%i -- %s",list_counts[j],list[j]);
        strcpy(list_with_cnts[j],tmp); 
        j++;
      }
      draw_text(&tft,"LIST",list_with_cnts,array_size(list_with_cnts),selection);
      break;
    case Item:
      char item_str[150];
      sprintf(item_str,"%s%s", ITEM_TEXT, item);
      sprintf(selection_str,"%i",selection); 
      tft.println(item_str);
      tft.println(selection_str);
      break;
    case FindStores:
        tft.println(stores);
        break;
  }
}

int array_size(char arr[][MAX_OPTION_LEN]){
  int i = 0;
  while(strcmp(arr[i],"")!=0 && i < MAX_OPTION_LEN)
    i++;
  return i;
}

void update_state(uint8_t b1, uint8_t b2){
  prev_state = display_state;
  prev_selection = selection;
  if(b2 == 1){ //long press returns to start menu
    state = Menu;
    display_state = Menu;
    change_display = true;
    selection = 0;
    return;
  }
  
  switch(state){
    case Menu:
      if (b1 == 1){
        make_selection(MENU_OPTION_STATES, sizeof(MENU_OPTION_STATES)/sizeof(MENU_OPTION_STATES[0]),&selection);
        if(state == EditList){
          get_items(list,UPC_list,list_counts,MAX_LIST_LEN,username);
        }
        else if(state == FindStores){
          get_stores(&my_gps,stores);
        }
        selection = 0;
      }
      else
        update_selection(&my_imu,&selection,sizeof(MENU_OPTIONS)/sizeof(MENU_OPTIONS[0]));
      break;
    case AddItem:
      if(b1 == 1){
        state = ProcessImage;
        begin_take_photo = true;
      }
      break;
    case ProcessImage:
      if(!begin_take_photo) //done taking photo
          state = ConfirmImage;
      break;
    case ConfirmImage:
        if (b1 == 1){
          make_selection(CONFIRM_OPTION_STATES, sizeof(CONFIRM_OPTION_STATES)/sizeof(CONFIRM_OPTION_STATES[0]),&selection);
          selection = 0;
        }
        else
          update_selection(&my_imu,&selection,sizeof(CONFIRM_OPTIONS)/sizeof(CONFIRM_OPTIONS[0]));
        break;
    case ItemQuantity:
       if(b1 == 1){
          sprintf(success_message,"You added %i of item %s",selection,item);
          add_item(username,item,selection,UPC, &my_gps);
          state = Success;
          state_timer = millis();
          selection = 0;
        }
        else
          update_selection(&my_imu,&selection,MAX_ITEM_QUANTITY);
        break;
    case Success:
        if(millis()-state_timer > SUCCESS_SCREEN_DURATION)
           state = AddItem;
        break;
    case EditList:
      //add in scrolling
      if(b1 == 1){
         state = Item;
         strcpy(item,list[selection]);
         item_index = selection;
         selection = 0;
      }
      else
        update_selection(&my_imu,&selection,array_size(list));
      break;
   case Item:
      if(b1 == 1){
        state = EditList;
        selection = selection - list_counts[item_index];
        add_item(username,item,selection,UPC, &my_gps); //make sure the database handles duplicates
        get_items(list,UPC_list,list_counts,MAX_LIST_LEN,username);
        selection = 0;
      }
      else
        update_selection(&my_imu,&selection,MAX_ITEM_QUANTITY);
      break;
   case FindStores:
      //add in scrolling
      break;
  }
  display_state = state;
  if(prev_state != display_state){
    change_display = true;
  }
}

void make_selection(State * option_states, int num_option_states, int * selection){
  state = option_states[*selection];
}
