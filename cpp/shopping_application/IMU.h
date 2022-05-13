#ifndef IMU_h
#define IMU_h
#include "Arduino.h"
#include <mpu6050_esp32.h>
class IMU{
  MPU6050 * imu;
  public:
  IMU(MPU6050 * iMu);
  void get_angle(float* x, float* y);
};
#endif
