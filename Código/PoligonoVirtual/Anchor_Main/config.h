#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ETH.h>

#define ANCHOR_ID 0
#define ANCHOR_IS_MASTER 1

#define MAX_TAGS 40
#define MAX_BUFFERED_PACKETS 256

#define UDP_PORT 5000
#define SERVER_IP IPAddress(192,168,1,100)

#define SYNC_INTERVAL_MS 100
#define TAG_SLOT_US 2000        // 2ms slot
#define FRAME_PERIOD_MS 100     // 100ms frame

#define DW_IRQ_PIN 34
#define DW_RST_PIN 27
#define DW_CS_PIN 5

#endif