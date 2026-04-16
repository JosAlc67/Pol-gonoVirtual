#include "config.h"
#include "display_manager.h"
#include "uwb_manager.h"

unsigned long last_blink_time = 0;

void setup() {
    Serial.begin(115200);
    
    // Inicializar módulos
    display_init(TAG_ID);
    uwb_init();
    
    Serial.println("Tag Chaleco Inicializado. Iniciando balizas UWB...");
}

void loop() {
    unsigned long current_time = millis();

    // Loop no bloqueante a 10 Hz (BLINK_INTERVAL = 100ms)
    if (current_time - last_blink_time >= BLINK_INTERVAL) {
        last_blink_time = current_time;
        
        display_update_status(true);
        uwb_transmit_blink();
        display_update_status(false);
    }
    
    // Espacio para futura integración del stack WiFi (Fase 2)
}