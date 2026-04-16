#ifndef UWB_DRIVER_H
#define UWB_DRIVER_H

#include <Arduino.h>
#include "dw3000.h"

void uwbInit();
void uwbStartRX();
bool uwbPacketAvailable();
void uwbGetPacket(uint8_t *buffer, uint16_t *len);
uint64_t uwbGetRxTimestamp();
void uwbSendFrame(uint8_t *data, uint16_t len);

#endif