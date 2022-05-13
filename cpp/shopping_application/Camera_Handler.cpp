#include "Arduino.h"
#include "Camera_Handler.h"

void init_camera(ArduCAM * myCAM, int cs, int cam_power_on){
  pinMode(CS,OUTPUT);
  pinMode(CAM_POWER_ON , OUTPUT);
  digitalWrite(CAM_POWER_ON, HIGH);
  #if defined(__SAM3X8E__)
    Wire1.begin();
  #else
    Wire.begin();
  #endif

  SPI.begin();
  SPI.setFrequency(10000000); //4MHz //must change in User Setup File for TFT LCD Screen in arduino libraries

  uint8_t vid, pid;
  uint8_t temp;
  
  //Check if the ArduCAM SPI bus is OK
  myCAM->write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM->read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55){
    Serial.println(F("SPI1 interface Error!"));
    while(1);
  }

  myCAM->wrSensorReg8_8(0xff, 0x01);
  myCAM->rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM->rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
  if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
    Serial.println(F("Can't find OV2640 module!"));
  else
    Serial.println(F("OV2640 detected."));
  
  //Check if the ArduCAM SPI bus is OK
  myCAM->write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM->read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55){
    Serial.println(F("SPI1 interface Error!"));
    while(1);
  }
  
  //Change to BMP capture mode and initialize the OV2640 module
  myCAM->set_format(BMP);
  myCAM->InitCAM();
  myCAM->OV2640_set_JPEG_size(OV2640_800x600);//changed
  myCAM->clear_fifo_flag();
}

void change_format(ArduCAM * myCAM, byte fmt){
  if(fmt == JPEG){
    myCAM->wrSensorRegs8_8(OV2640_JPEG_INIT);
    myCAM->wrSensorRegs8_8(OV2640_YUV422);
    myCAM->wrSensorRegs8_8(OV2640_JPEG);
    myCAM->wrSensorReg8_8(0xff, 0x01);
    myCAM->wrSensorReg8_8(0x15, 0x00);
    myCAM->wrSensorRegs8_8(OV2640_320x240_JPEG);
  }
  else{
    myCAM->wrSensorRegs8_8(OV2640_QVGA);
  }
}

void show_image(ArduCAM * myCAM, TFT_eSPI * tft, uint16_t * bitmap){
  myCAM->OV2640_set_JPEG_size(OV2640_160x120);
  start_capture(myCAM);
  while (!myCAM->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  camDisplay(myCAM, tft, bitmap);
  memset(bitmap,0,BITMAP_SIZE);
}

void take_photo(ArduCAM * myCAM){
  change_format(myCAM,JPEG);
  myCAM->OV2640_set_JPEG_size(OV2640_352x288);
  delay(1000);
  Serial.println("TAKING PHOTO");
  start_capture(myCAM);
  Serial.println("END CAPTURE");
  while (!myCAM->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  Serial.println("Uploading...");
  camCapture(myCAM);
  change_format(myCAM,BMP);
}

void camDisplay(ArduCAM * myCAM, TFT_eSPI * tft, uint16_t * bitmap){
  myCAM->CS_LOW();
  myCAM->set_fifo_burst(); 
  uint8_t temp = 0, temp_last = 0;
  int i = 0;
  while(i++ < 2*BITMAP_SIZE){
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    if(i%2==1){
      bitmap[(i-1)/2] = (temp_last << 8) | temp;
    }
  }
  myCAM->CS_HIGH();
  tft->pushImage(0,0,160,120,bitmap);
}

void start_capture(ArduCAM * myCAM){
  myCAM->clear_fifo_flag();
  myCAM->start_capture();
}

void camCapture(ArduCAM * myCAM){
  static int camera_buffer[camera_buffer_size];
  static char str_buffer[4*camera_buffer_size];
  memset(camera_buffer,0,camera_buffer_size);
  strcpy(str_buffer,"");
  
  uint32_t len  = myCAM->read_fifo_length();
  Serial.println(len);
  myCAM->CS_LOW();
  myCAM->set_fifo_burst(); 
  
  uint8_t temp = 0, temp_last = 0;
  int i = 0;
  boolean is_header = false;
  boolean new_photo = true;
  while ( len-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    //Serial.println(temp);
    //Read JPEG data from FIFO 
    if(is_header)
    { 
      if (i == camera_buffer_size){
        ints_to_string(camera_buffer, i, str_buffer);
        send_buffer(str_buffer,new_photo); //changed
        new_photo = false;
        memset(camera_buffer,0,camera_buffer_size);
        strcpy(str_buffer,"");
        i = 0;
      }
      camera_buffer[i++] = temp;      
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      camera_buffer[i++] = temp_last;
      camera_buffer[i++] = temp;   
    }
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      break; 
    }  
  }
  ints_to_string(camera_buffer, i, str_buffer);
  send_buffer(str_buffer); //changed
  myCAM->CS_HIGH(); 
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

void send_buffer(char * str_buffer,bool new_photo){
  static char camera_request_buffer[CAMERA_IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
  static char camera_response_buffer[CAMERA_OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response
  static char camera_request_body[CAMERA_IN_BUFFER_SIZE];
  strcpy(camera_request_buffer,"");
  strcpy(camera_response_buffer,"");
  strcpy(camera_request_body,"");
  sprintf(camera_request_body,"{\"new\":\"%i\",\"photo\":\"",new_photo);
  strcat(camera_request_body,str_buffer);
  strcpy(camera_request_body+strlen(camera_request_body),"\"}");//generate camera_request_body, posting to User, 1 step
  int camera_request_body_len = strlen(camera_request_body); //calculate camera_request_body length (for header reporting)
  sprintf(camera_request_buffer,"POST http://608dev-2.net/sandbox/sc/team114/final_project/process_image/Store_Image.py HTTP/1.1\r\n");
  strcat(camera_request_buffer,"Host: 608dev-2.net\r\n");
  strcat(camera_request_buffer,"Content-Type: application/json\r\n");
  sprintf(camera_request_buffer+strlen(camera_request_buffer),"Content-Length: %d\r\n", camera_request_body_len); //append string formatted to end of request buffer
  strcat(camera_request_buffer,"\r\n"); //new line from header to camera_request_body
  strcat(camera_request_buffer,camera_request_body); //camera_request_body
  strcat(camera_request_buffer,"\r\n"); //header
  do_http_request("608dev-2.net", camera_request_buffer, camera_response_buffer, CAMERA_OUT_BUFFER_SIZE, CAMERA_RESPONSE_TIMEOUT,false);
  Serial.println(camera_response_buffer);
}
