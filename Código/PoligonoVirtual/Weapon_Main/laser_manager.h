#ifndef LASER_MANAGER_H
#define LASER_MANAGER_H

#include <Arduino.h>

void laser_init();
bool laser_check_fire_event(uint64_t* out_timestamp);

#endif