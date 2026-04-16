#include "wifi_manager.h"
#include "config_weapon.h"
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp_client;

#pragma pack(push, 1)
struct ShotEventPacket {
    uint16_t header = 0xBB66;
    uint16_t weapon_id;
    uint64_t fire_timestamp;
    float q_w, q_x, q_y, q_z;
};
#pragma pack(pop)

void wifi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    // Bucle no bloqueante recomendado en el main para no trabar el EKF, 
    // pero se inicializa aquí el estado de red.
}

void wifi_send_shot_event(uint16_t weapon_id, uint64_t timestamp, OrientationRecord record) {
    if (WiFi.status() == WL_CONNECTED) {
        ShotEventPacket packet;
        packet.weapon_id = weapon_id;
        packet.fire_timestamp = timestamp;
        packet.q_w = record.q_w;
        packet.q_x = record.q_x;
        packet.q_y = record.q_y;
        packet.q_z = record.q_z;

        udp_client.beginPacket(SERVER_IP, SERVER_WIFI_PORT);
        udp_client.write((uint8_t*)&packet, sizeof(ShotEventPacket));
        udp_client.endPacket();
    }
}