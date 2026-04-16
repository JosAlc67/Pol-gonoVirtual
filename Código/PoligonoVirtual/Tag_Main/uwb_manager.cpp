#include "uwb_manager.h"
#include "config.h"
#include <SPI.h>

// Librerías específicas de tu repositorio DW3000
#include "dw3000.h" 
#include "dw3000_port.h"
#include "dw3000_device_api.h"
#include "dw3000_config_options.h" // Contiene 'config_options' predefinido

static uint8_t sequence_number = 0;

// Trama MAC ajustada (Minimizada para TDoA)
static uint8_t tx_blink_msg[] = {
  0x41, 0x88,           // Frame Control (Data, Broadcast)
  0,                    // Sequence number (MAC)
  0xCA, 0xDE,           // PAN ID
  0xFF, 0xFF,           // Dest Address (Broadcast)
  0x00, 0x00,           // Source Address (Se llena en init)
  TAG_TYPE,             // Payload: Tipo (Chaleco)
  0x00,                 // Payload: Secuencia interna
  0, 0                  // 2 bytes reservados para CRC (HW DW3000 lo calcula)
};

void uwb_init() {
    // 1. Inicializar bus SPI en los pines definidos para el ESP32-S3
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, DW_CS);

    // 2. Inicializar pines y hardware UWB según la librería (dw3000_port.cpp)
    // spiBegin configura el pin IRQ como entrada y el pin de Reset
    spiBegin(DW_IRQ, DW_RST);
    
    // spiSelect activa el chip, hace un reset por hardware, limpia interrupciones,
    // configura la red por defecto y carga el microcódigo LDE en RAM.
    spiSelect(DW_CS);
    delay(2); // Retardo de estabilización post-inicio

    // 3. Configurar PHY UWB (Canal, PRF, Preamble)
    // Utiliza la estructura 'config_options' ya instanciada por la librería
    dwt_configure(&config_options);

    // 4. Asignar ID dinámico a la dirección de origen en la trama
    tx_blink_msg[7] = TAG_ID & 0xFF;
    tx_blink_msg[8] = (TAG_ID >> 8) & 0xFF;
}

void uwb_transmit_blink() {
    // Actualizar números de secuencia
    tx_blink_msg[2] = sequence_number;       
    tx_blink_msg[10] = sequence_number;      
    
    // Escribir los datos en el buffer de transmisión (TX_BUFFER_ID)
    dwt_writetxdata(sizeof(tx_blink_msg), tx_blink_msg, 0);
    
    // Configurar control de la trama (Longitud, Offset = 0, Ranging = 0)
    dwt_writetxfctrl(sizeof(tx_blink_msg), 0, 0);
    
    // Disparar la transmisión inmediata
    dwt_starttx(DWT_START_TX_IMMEDIATE);
    
    // Bucle de espera no bloqueante: leer el registro de estado del sistema (SYS_STATUS) 
    // y esperar hasta que el bit de Transmisión Completada (TXFRS) sea igual a 1.
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS_BIT_MASK)) { 
        // Esperando a que el paquete viaje por el aire
    };
    
    // Limpiar el flag en el registro escribiendo un 1 en esa misma posición
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
    
    sequence_number++;
}