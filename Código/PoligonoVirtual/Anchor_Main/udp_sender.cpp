#include "udp_sender.h"

static AggregatedUDPPacket batch_packet;
static unsigned long last_batch_time = 0;
static const unsigned long BATCH_INTERVAL_MS = 500;

void udp_sender_init(uint8_t anchor_id) {
    batch_packet.header = 0xAA55;
    batch_packet.anchor_id = anchor_id;
    batch_packet.tag_count = 0;
    batch_packet.crc = 0;
}

void udp_sender_add_tag(uint16_t id, uint8_t type, uint32_t seq, uint64_t ts, int64_t offset) {
    if (batch_packet.tag_count < MAX_TAGS_PER_BATCH) {
        uint8_t index = batch_packet.tag_count;
        
        batch_packet.tags[index].tag_id = id;
        batch_packet.tags[index].tag_type = type;
        batch_packet.tags[index].sequence = seq;
        batch_packet.tags[index].rx_timestamp = ts;
        batch_packet.tags[index].sync_offset = offset;
        
        batch_packet.tag_count++;
    }
}

void udp_sender_tick() {
    unsigned long current_time = millis();

    // Se evalúa si ya se cumplió la ventana de 500 ms
    if (current_time - last_batch_time >= BATCH_INTERVAL_MS) {
        last_batch_time = current_time;
        
        if (batch_packet.tag_count > 0) {
            
            // Aquí iría la llamada a la librería Ethernet real
            // ethernet_send_udp((uint8_t*)&batch_packet, sizeof(AggregatedUDPPacket));
            
            Serial.print("Lote UDP enviado. Total de tags en la ventana: ");
            Serial.println(batch_packet.tag_count);
            
            // Limpieza lógica: reiniciar el contador para la siguiente ventana
            batch_packet.tag_count = 0; 
        }
    }
}