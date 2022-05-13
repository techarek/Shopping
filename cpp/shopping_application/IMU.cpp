#include "Arduino.h"
#include "IMU.h"

IMU::IMU(MPU6050 * iMu){
  imu = iMu;
}

void IMU::get_angle(float* x, float* y){
  imu->readAccelData(imu->accelCount);
  *x = imu->accelCount[0] * imu->aRes;
  *y = imu->accelCount[1] * imu->aRes;
}
