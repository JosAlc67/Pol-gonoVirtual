#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include <Arduino.h>

void initSync();
void processSync();
void handleSyncFrame(uint8_t *data);
int64_t getSyncOffset();

#endif