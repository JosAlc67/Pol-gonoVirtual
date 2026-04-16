#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

void display_init(uint16_t tag_id);
void display_update_status(bool is_transmitting);

#endif // DISPLAY_MANAGER_H