#ifndef IMU_EKF_MANAGER_H
#define IMU_EKF_MANAGER_H

#include <Arduino.h>

struct OrientationRecord {
    uint64_t timestamp;
    float q_w, q_x, q_y, q_z;
};

void imu_ekf_init();
void imu_ekf_update();
bool imu_get_historical_orientation(uint64_t target_timestamp, OrientationRecord* out_record);

#endif