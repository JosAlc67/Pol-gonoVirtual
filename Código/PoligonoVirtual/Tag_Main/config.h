#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- Parámetros del Tag ---
#define TAG_ID          5        // ID único del chaleco
#define TAG_TYPE        0x02     // 0x01 = Arma, 0x02 = Chaleco

// Se ajusta la ventana de transmisión a 500 ms para coincidir con la arquitectura
#define BLINK_INTERVAL  500      

// --- Pines SPI para DW3000 ---
#define SPI_SCK         36
#define SPI_MISO        37
#define SPI_MOSI        35
#define DW_CS           34
#define DW_RST          27       
#define DW_IRQ          38       

#endif // CONFIG_H