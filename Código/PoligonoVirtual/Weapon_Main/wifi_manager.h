#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include "imu_ekf_manager.h"

void wifi_init();
void wifi_send_shot_event(uint16_t weapon_id, uint64_t timestamp, OrientationRecord record);

#endif