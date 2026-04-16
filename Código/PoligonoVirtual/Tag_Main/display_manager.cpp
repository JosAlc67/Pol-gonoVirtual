#include "display_manager.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); 

void display_init(uint16_t tag_id) {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    // Interfaz base
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawCentreString("CHALECO UWB", 80, 20, 4);
    
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    String id_str = "TAG ID: " + String(tag_id);
    tft.drawCentreString(id_str, 80, 60, 4);
}

void display_update_status(bool is_transmitting) {
    // Ejemplo: Mostrar un indicador visual cuando el tag está activo
    if(is_transmitting) {
        tft.fillCircle(10, 10, 5, TFT_GREEN);
    } else {
        tft.fillCircle(10, 10, 5, TFT_RED);
    }
}