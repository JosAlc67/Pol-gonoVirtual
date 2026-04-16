#include "uwb_handler.h"
#include "uwb_driver.h"
#include "ring_buffer.h"
#include "sync_manager.h"
#include "udp_sender.h"
#include "packet_structs.h"

// Variables estáticas locales, ya no necesitan ensuciar el archivo principal
static uint8_t uwb_rx_temp[256];
static uint16_t uwb_len = 0;

void uwb_handler_capture() {
    if (uwbPacketAvailable()) {
        uwbGetPacket(uwb_rx_temp, &uwb_len);
        uint64_t ts = uwbGetRxTimestamp();

        // Guardar en buffer circular de alta velocidad
        ringPush(uwb_rx_temp, uwb_len, ts);
    }
}

void uwb_handler_process_buffer() {
    UWBFrame frame;

    // Vaciamos el buffer circular
    while (ringPop(&frame)) {
        if (frame.len < 4) continue; // evitar basura

        // ---- Interpretación de la trama ----
        uint16_t tag_id = (frame.data[0] << 8) | frame.data[1];
        uint8_t tag_type = frame.data[2];
        uint8_t sequence = frame.data[3];

        // Cálculo de tiempos
        uint64_t absolute_timestamp = frame.timestamp + getSyncOffset();
        int64_t current_offset = getSyncOffset();

        // ---- Delegar al gestor de lotes UDP ----
        udp_sender_add_tag(tag_id, tag_type, sequence, absolute_timestamp, current_offset);
    }
}