#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <Arduino.h>

#define RING_SIZE 128

typedef struct
{
    uint8_t data[256];
    uint16_t len;
    uint64_t timestamp;
} UWBFrame;

void ringInit();
bool ringPush(uint8_t *data, uint16_t len, uint64_t ts);
bool ringPop(UWBFrame *frame);

#endif