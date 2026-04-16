#ifndef UDP_SENDER_H
#define UDP_SENDER_H

#include "packet_structs.h"

// Inicializa el módulo asignando el ID del ancla
void udp_sender_init(uint8_t anchor_id);

// Agrega un tag capturado al lote temporal
void udp_sender_add_tag(uint16_t id, uint8_t type, uint32_t seq, uint64_t ts, int64_t offset);

// Función no bloqueante que evalúa el tiempo y envía el paquete si pasaron 500ms
void udp_sender_tick();

#endif // UDP_SENDER_H