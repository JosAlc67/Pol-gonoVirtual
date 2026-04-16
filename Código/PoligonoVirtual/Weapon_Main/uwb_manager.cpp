#include "uwb_manager.h"
#include "config_weapon.h"
#include <SPI.h>
#include "dw3000.h" 
#include "dw3000_port.h"
#include "dw3000_device_api.h"
#include "dw3000_config_options.h"

static uint8_t sequence_number = 0;

static uint8_t tx_shot_msg[] = {
  0x41, 0x88,           
  0,                    
  0xCA, 0xDE,           
  0xFF, 0xFF,           
  0x00, 0x00,           
  TAG_TYPE,             
  0x00,                 
  0, 0                  
};

void uwb_weapon_init() {
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, DW_CS);
    spiBegin(DW_IRQ, DW_RST);
    spiSelect(DW_CS);
    delay(2); 

    dwt_configure(&config_options);

    tx_shot_msg[7] = WEAPON_ID & 0xFF;
    tx_shot_msg[8] = (WEAPON_ID >> 8) & 0xFF;
}

void uwb_transmit_shot() {
    tx_shot_msg[2] = sequence_number;       
    tx_shot_msg[10] = sequence_number;      
    
    dwt_writetxdata(sizeof(tx_shot_msg), tx_shot_msg, 0);
    dwt_writetxfctrl(sizeof(tx_shot_msg), 0, 0);
    dwt_starttx(DWT_START_TX_IMMEDIATE);
    
    // Al ser orientado a eventos, bloqueamos un par de microsegundos hasta que salga el disparo
    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS_BIT_MASK)) { };
    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
    
    sequence_number++;
}