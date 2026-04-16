#include "config_weapon.h"
#include "laser_manager.h"
#include "imu_ekf_manager.h"
#include "wifi_manager.h"
#include "uwb_manager.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=== WEAPON TAG START ===");

    laser_init();
    imu_ekf_init();
    uwb_weapon_init();
    wifi_init();

    Serial.println("=== SYSTEM READY ===");
}

void loop() {
    // 1. Motor EKF ejecutándose continuamente a alta frecuencia (100 Hz)
    imu_ekf_update();

    // 2. Revisión de hardware: ¿Se detectó el láser?
    uint64_t fire_timestamp;
    if (laser_check_fire_event(&fire_timestamp)) {
        
        Serial.println("EVENTO: Disparo detectado.");

        // 3. Compensación del tirón mecánico (Viaje en el tiempo)
        uint64_t target_time = fire_timestamp - TRIGGER_JERK_OFFSET_MS;
        
        OrientationRecord retro_record;
        if (imu_get_historical_orientation(target_time, &retro_record)) {
            
            // 4. Reportar orientación precisa vía WiFi al servidor
            wifi_send_shot_event(WEAPON_ID, fire_timestamp, retro_record);
            
            // 5. Reportar evento de posicionamiento TDoA a las Anclas
            uwb_transmit_shot();
            
            Serial.println("Datos balísticos despachados correctamente.");
        }
    }
}