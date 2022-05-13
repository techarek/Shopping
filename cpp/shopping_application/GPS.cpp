#include "Arduino.h"
#include "GPS.h"
#include <string.h>

GPS::GPS(HardwareSerial * gps_ptr){
    gps = gps_ptr;
    gps->begin(9600,SERIAL_8N1,32,33);
  }

void GPS::extractGNRMC(){
    char gps_buffer[GPS_BUFFER_LENGTH] = {0}; //dump chars into the 
    if(gps->available()) {     // If anything comes in Serial1 (pins 0 & 1)
      gps->readBytesUntil('\n', gps_buffer, GPS_BUFFER_LENGTH); // read it and send it out Serial (USB)
      char* info = strstr(gps_buffer,"GNRMC");
      if (info!=NULL){
        Serial.println(gps_buffer);
        extract(gps_buffer);
      }
    }
  }

void GPS::extract(char* data_array){
    if(strstr(data_array,",V,") != NULL) return;
    
    char temp[10] = "";
    char* data_segment = strtok(data_array,",");
    data_segment = strtok(NULL,",");
    hour = atoi(strncat(temp,data_segment,2));
    strcpy(temp,"");
    data_segment += 2;
    minute = atoi(strncat(temp,data_segment,2));
    strcpy(temp,"");
    data_segment += 2;
    second = atoi(strncat(temp,data_segment,2));
    strcpy(temp,"");
    data_segment = strtok(NULL,",");
    valid = strcmp(data_segment,"A")==0;
    if(!valid) return;
    data_segment = strtok(NULL,",");
    lat_deg = atoi(strncat(temp,data_segment,2));
    strcpy(temp,"");
    data_segment += 2;
    lat_dm = atof(data_segment);
    data_segment = strtok(NULL,",");
    lat_dir = data_segment[0];
    data_segment = strtok(NULL,",");
    lon_deg = atoi(strncat(temp,data_segment,3));
    strcpy(temp,"");
    data_segment += 3;
    lon_dm = atof(data_segment);
    data_segment = strtok(NULL,",");
    lon_dir = data_segment[0];
    data_segment = strtok(NULL,",");
    data_segment = strtok(NULL,",");
    data_segment = strtok(NULL,",");
    day = atoi(strncat(temp,data_segment,2));
    strcpy(temp,"");
    data_segment += 2;
    month = atoi(strncat(temp,data_segment,2));
    strcpy(temp,"");
    data_segment += 2;
    year = atoi(strncat(temp,data_segment,2));
    strcpy(temp,"");
  }
