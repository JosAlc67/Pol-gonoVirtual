#ifndef CONFIG_WEAPON_H
#define CONFIG_WEAPON_H

#include <Arduino.h>

// --- Identificación ---
#define WEAPON_ID       10
#define TAG_TYPE        0x01

// --- Pines de Hardware ---
#define PIN_LASER_IN    4
#define PIN_I2C_SDA     8
#define PIN_I2C_SCL     9

// --- Pines DW3000 (UWB) ---
#define SPI_SCK         36
#define SPI_MISO        37
#define SPI_MOSI        35
#define DW_CS           34
#define DW_RST          27
#define DW_IRQ          38

// --- Parámetros de Red WiFi ---
#define WIFI_SSID       "Poligono_AP"
#define WIFI_PASS       "password123"
#define SERVER_IP       "192.168.1.100"
#define SERVER_WIFI_PORT 8080

// --- Parámetros de Tiempo y Buffer ---
#define IMU_SAMPLE_RATE_HZ   100  // 100 Hz es óptimo para el EKF y BMI270
#define IMU_SAMPLE_DELAY_MS  (1000 / IMU_SAMPLE_RATE_HZ)
#define HISTORY_BUFFER_SIZE  50   // 500 ms de historial
#define TRIGGER_JERK_OFFSET_MS 40 // Retroceder 40ms al disparar

#endif // CONFIG_WEAPON_H