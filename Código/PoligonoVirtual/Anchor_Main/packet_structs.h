#ifndef PACKET_STRUCTS_H
#define PACKET_STRUCTS_H

#include <Arduino.h>

#define MAX_TAGS_PER_BATCH 40

struct UwbPacket {
    uint16_t tag_id;
    uint8_t  tag_type;
    uint32_t sequence;
    uint64_t timestamp;
};

struct TagData {
    uint16_t tag_id;
    uint8_t  tag_type;
    uint32_t sequence;
    uint64_t rx_timestamp;
    int64_t  sync_offset;
} __attribute__((packed));

struct AggregatedUDPPacket {
    uint16_t header;       
    uint8_t  anchor_id;    
    uint8_t  tag_count;    
    TagData  tags[MAX_TAGS_PER_BATCH]; 
    uint16_t crc;          
} __attribute__((packed));

#endif // PACKET_STRUCTS_H