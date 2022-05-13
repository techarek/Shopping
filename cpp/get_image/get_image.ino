#include <WiFi.h>
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "memorysaver.h"
#include "button.h"

// READ ME: To use, make sure to change the wifi network to your local wifi. Additionally, check that you have a button connected to pin 16. To focus camera, use show_image() method.
// Once the camera is focused, comment out show_image in the loop method in order to prevent blocking (bug that needs to be fixed). Additionally, make sure your user_setup.h file
// for your TFT library has an SPI_FREQUENCY of 12 MHz

const char network[] = "SpectrumSetup-0F";
const char password[] = "mirrortown456";

const int RESPONSE_TIMEOUT = 10000; //ms to wait for response from host

static const size_t bufferSize = 1500;
static int buffer[bufferSize];
char str_buffer[4*bufferSize];

const uint16_t IN_BUFFER_SIZE = 4*bufferSize + 500; //size of buffer to hold HTTP request
const uint16_t OUT_BUFFER_SIZE = 4*bufferSize + 500; //size of buffer to hold HTTP response
char request_buffer[IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response_buffer[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response
char body[IN_BUFFER_SIZE];

// set GPIO17 as the slave select :
const int CS = 17;
const int CAM_POWER_ON = 10;
ArduCAM myCAM(OV2640, CS);

TFT_eSPI tft = TFT_eSPI(); 

const int BUTTON_PIN = 16;
Button button(BUTTON_PIN);

const long BITMAP_SIZE = 19200;
uint16_t bitmap[BITMAP_SIZE];

uint8_t temp = 0, temp_last = 0;
int i = 0;
bool is_header = false;


void start_capture(){
  myCAM.clear_fifo_flag();
  myCAM.start_capture();
}

void camCapture(ArduCAM myCAM){
  uint32_t len  = myCAM.read_fifo_length();
  Serial.println(len);
  myCAM.CS_LOW();
  myCAM.set_fifo_burst(); 
  i = 0;
  is_header = false;
  while ( len-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    Serial.println(temp);
    //Read JPEG data from FIFO 
    if(is_header)
    { 
      if (i == bufferSize){
        ints_to_string(buffer, i, str_buffer);
        send_buffer(); //changed
        i = 0;
      }
      buffer[i++] = temp;      
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      buffer[i++] = temp_last;
      buffer[i++] = temp;   
    }
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      break; 
    }  
  }
  ints_to_string(buffer, i, str_buffer);
  send_buffer(); //changed
  myCAM.CS_HIGH(); 
}

void ints_to_string(int* input, int input_length, char * output){
  int i = 0;
  while(i < input_length){
    char addendum[10];
    sprintf(addendum,"%i,",input[i]);
    strcpy(output + strlen(output),addendum);
    i++;
  }    
}

void camDisplay(){
  myCAM.CS_LOW();
  myCAM.set_fifo_burst(); 
  i = 0;
  while(i < 2*BITMAP_SIZE){
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    if(i%2==1){
      bitmap[(i-1)/2] = (temp << 8) | temp_last;
    }
    i++;
  }
  myCAM.CS_HIGH();
  tft.pushImage(0,0,160,120,bitmap);
}

void send_buffer(){
  sprintf(body,"{\"photo\":\"");
  strcat(body,str_buffer);
  strcpy(body+strlen(body),"\"}");//generate body, posting to User, 1 step
  int body_len = strlen(body); //calculate body length (for header reporting)
  sprintf(request_buffer,"POST http://608dev-2.net/sandbox/sc/team114/final_project/process_image/Store_Image.py HTTP/1.1\r\n");
  strcat(request_buffer,"Host: 608dev-2.net\r\n");
  strcat(request_buffer,"Content-Type: application/json\r\n");
  sprintf(request_buffer+strlen(request_buffer),"Content-Length: %d\r\n", body_len); //append string formatted to end of request buffer
  strcat(request_buffer,"\r\n"); //new line from header to body
  strcat(request_buffer,body); //body
  strcat(request_buffer,"\r\n"); //header
  do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,false);
  Serial.println(response_buffer);
  strcpy(response_buffer,"");
  strcpy(request_buffer,"");
  strcpy(str_buffer,"");
  strcpy(body,"");
  memset(buffer, 0, sizeof(buffer));
}

void take_photo(){//changed: commented out
  change_format(JPEG);
  myCAM.OV2640_set_JPEG_size(OV2640_800x600);
  delay(700);
  Serial.println("TAKING PHOTO");
  start_capture();
  Serial.println("END CAPTURE");
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  Serial.println("Uploading...");
  camCapture(myCAM);
  change_format(BMP);
}

void show_image(){
  myCAM.OV2640_set_JPEG_size(OV2640_160x120);
  start_capture();
  Serial.print("here1");
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  Serial.print("here");
  camDisplay();
}

void change_format(byte fmt){
  //myCAM.set_format(fmt);
  //myCAM.InitCAM();
  if(fmt == JPEG){
    myCAM.wrSensorRegs8_8(OV2640_JPEG_INIT);
    myCAM.wrSensorRegs8_8(OV2640_YUV422);
    myCAM.wrSensorRegs8_8(OV2640_JPEG);
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.wrSensorReg8_8(0x15, 0x00);
    myCAM.wrSensorRegs8_8(OV2640_320x240_JPEG);
  }
  else{
    myCAM.wrSensorRegs8_8(OV2640_QVGA);
  }
}

void get_product_description(){
  sprintf(request_buffer,"GET http://608dev-2.net/sandbox/sc/team114/final_project/process_image/Process_Image.py?num=0&path=None HTTP/1.1\r\n");
  strcat(request_buffer,"Host: 608dev-2.net\r\n");
  strcat(request_buffer,"\r\n"); //new line from header to body
  do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
  Serial.println(response_buffer);
  strcpy(response_buffer,"");
  strcpy(request_buffer,"");
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  uint8_t vid, pid;
  uint8_t temp;
    //set the CS as an output:
    pinMode(CS,OUTPUT);
    pinMode(CAM_POWER_ON , OUTPUT);
    digitalWrite(CAM_POWER_ON, HIGH);
   #if defined(__SAM3X8E__)
    Wire1.begin();
    #else
    Wire.begin();
    #endif

  pinMode(BUTTON_PIN,INPUT_PULLUP);

  strcpy(str_buffer,"");
  strcpy(response_buffer,"");
  strcpy(request_buffer,"");
  
  // initialize SPI:
  SPI.begin();
  SPI.setFrequency(12000000); //4MHz //must change in User Setup File for TFT LCD Screen in arduino libraries

  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55){
  Serial.println(F("SPI1 interface Error!"));
  while(1);
  }

  myCAM.wrSensorReg8_8(0xff, 0x01);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
  if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
  Serial.println(F("Can't find OV2640 module!"));
  else
  Serial.println(F("OV2640 detected."));
  
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55){
  Serial.println(F("SPI1 interface Error!"));
  while(1);
  }
  
  //Change to JPEG capture mode and initialize the OV2640 module
  myCAM.set_format(BMP);
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_800x600);//changed
  myCAM.clear_fifo_flag();

  connect_wifi();
}

void loop() {
  uint8_t flag = button.update();
  show_image();//changed
  if(flag==1){
     take_photo();
     get_product_description();
    }
  }

void connect_wifi(){
  WiFi.begin(network,password); //attempt to connect to wifi
  uint8_t count = 0; //count used for Wifi check times
  Serial.print("Attempting to connect to ");
  Serial.println(network);
  while (WiFi.status() != WL_CONNECTED && count<12) {
    delay(500);
    Serial.print(".");
    count++;
  }
  delay(2000);
  if (WiFi.isConnected()) { //if we connected then print our IP, Mac, and SSID we're on
    Serial.println("CONNECTED!");
    Serial.println(WiFi.localIP().toString() + " (" + WiFi.macAddress() + ") (" + WiFi.SSID() + ")");
    delay(500);
  } else { //if we failed to connect just Try again.
    Serial.println("Failed to Connect :/  Going to restart");
    Serial.println(WiFi.status());
    ESP.restart(); // restart the ESP (proper way)
  }
}

/*----------------------------------
 * char_append Function:
 * Arguments:
 *    char* buff: pointer to character array which we will append a
 *    char c: 
 *    uint16_t buff_size: size of buffer buff
 *    
 * Return value: 
 *    boolean: True if character appended, False if not appended (indicating buffer full)
 */
uint8_t char_append(char* buff, char c, uint16_t buff_size) {
        int len = strlen(buff);
        if (len>buff_size) return false;
        buff[len] = c;
        buff[len+1] = '\0';
        return true;
}

/*----------------------------------
 * do_http_request Function:
 * Arguments:
 *    char* host: null-terminated char-array containing host to connect to
 *    char* request: null-terminated char-arry containing properly formatted HTTP request
 *    char* response: char-array used as output for function to contain response
 *    uint16_t response_size: size of response buffer (in bytes)
 *    uint16_t response_timeout: duration we'll wait (in ms) for a response from server
 *    uint8_t serial: used for printing debug information to terminal (true prints, false doesn't)
 * Return value:
 *    void (none)
 */
void do_http_request(char* host, char* request, char* response, uint16_t response_size, uint16_t response_timeout, uint8_t serial){
  WiFiClient client; //instantiate a client object
  if (client.connect(host, 80)) { //try to connect to host on port 80
    if (serial) Serial.print(request);//Can do one-line if statements in C without curly braces
    client.print(request);
    memset(response, 0, response_size); //Null out (0 is the value of the null terminator '\0') entire buffer
    uint32_t count = millis();
    while (client.connected()) { //while we remain connected read out data coming back
      client.readBytesUntil('\n',response,response_size);
      if (serial) Serial.println(response);
      if (strcmp(response,"\r")==0) { //found a blank line!
        break;
      }
      memset(response, 0, response_size);
      if (millis()-count>response_timeout) break;
    }
    memset(response, 0, response_size);  
    count = millis();
    while (client.available()) { //read out remaining text (body of response)
      char_append(response,client.read(),OUT_BUFFER_SIZE);
    }
    if (serial) Serial.println(response);
    client.stop();
    delay(100);
    if (serial) Serial.println("-----------");  
  }else{
    if (serial) Serial.println("connection failed :/");
    if (serial) Serial.println("wait 0.5 sec...");
    client.stop();
  }
}        
