#include "laser_manager.h"
#include "config_weapon.h"

volatile bool flag_shot_fired = false;
volatile uint64_t isr_fire_timestamp = 0;

void IRAM_ATTR on_laser_fired() {
    isr_fire_timestamp = millis();
    flag_shot_fired = true;
}

void laser_init() {
    pinMode(PIN_LASER_IN, INPUT); 
    attachInterrupt(digitalPinToInterrupt(PIN_LASER_IN), on_laser_fired, RISING);
}

bool laser_check_fire_event(uint64_t* out_timestamp) {
    if (flag_shot_fired) {
        *out_timestamp = isr_fire_timestamp;
        flag_shot_fired = false;
        return true;
    }
    return false;
}