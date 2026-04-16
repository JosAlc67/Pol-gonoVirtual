#include "config.h"
#include "ethernet_manager.h"
#include "udp_sender.h"
#include "uwb_driver.h"
#include "sync_manager.h"
#include "ring_buffer.h"
#include "uwb_handler.h" // Se incluye el nuevo módulo

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("=== ANCHOR START ===");

    initEthernet();
    udp_sender_init(ANCHOR_ID); 
    ringInit();
    uwbInit();
    initSync();

    Serial.println("=== SYSTEM READY ===");
}

void loop()
{
    // 1️⃣ Mantener el reloj sincronizado
    processSync();

    // 2️⃣ Capturar señales de radiofrecuencia entrantes
    uwb_handler_capture();

    // 3️⃣ Procesar colas y empaquetar datos
    uwb_handler_process_buffer();

    // 4️⃣ Motor de red (Evalúa envío cada 500ms)
    udp_sender_tick();
}