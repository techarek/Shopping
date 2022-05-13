#ifndef GPS_h
#define GPS_h
#include "Arduino.h"
class GPS{
  HardwareSerial *gps; //instantiate approporiate Serial object for GPS
  public:
  int lat_deg; //degrees portion of lattitude
  float lat_dm; //latitude decimal minutes
  char lat_dir; //latitude direction
  int lon_deg; //longitude in degrees
  float lon_dm; //longitude decimal minutes
  char lon_dir; //longitude direction
  int year; //year
  int month; //month 
  int day; //day of month
  int hour; //hour (24 clock GMT)
  int minute; //minute
  int second; //second
  const int GPS_BUFFER_LENGTH = 200;  //size of char array we'll use for
  bool valid; //is the data valid
  GPS(HardwareSerial * gps_ptr);
  void extractGNRMC();
  private:
  void extract(char* data_array);
};
#endif
