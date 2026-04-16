#include "uwb_driver.h"
#include "config.h"

#define RX_BUFFER_SIZE 256

static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint16_t rx_len = 0;
static volatile bool packet_ready = false;

static void rx_ok_cb(const dwt_cb_data_t *cb_data)
{
    rx_len = cb_data->datalength;

    if (rx_len < RX_BUFFER_SIZE)
    {
        dwt_readrxdata(rx_buffer, rx_len, 0);
        packet_ready = true;
    }

    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

static void rx_err_cb(const dwt_cb_data_t *cb_data)
{
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

void uwbInit()
{
    SPI.begin();

    if (dwt_initialise(DWT_DW_INIT) == DWT_ERROR)
    {
        Serial.println("DW3000 init failed");
        while (1);
    }

    dwt_config_t config = {
        5,              // Channel
        DWT_PLEN_128,
        DWT_PAC8,
        9,              // TX preamble code
        9,              // RX preamble code
        1,
        DWT_BR_6M8,
        DWT_PHRMODE_STD,
        DWT_PHRRATE_STD,
        (129 + 8 - 8)
    };

    dwt_configure(&config);

    dwt_setcallbacks(NULL, rx_ok_cb, NULL, rx_err_cb, NULL, NULL);

    dwt_setinterrupt(
        DWT_INT_RFCG |
        DWT_INT_RFCE |
        DWT_INT_RPHE,
        0,
        DWT_ENABLE_INT
    );

    dwt_rxenable(DWT_START_RX_IMMEDIATE);

    Serial.println("DW3000 ready");
}

void uwbStartRX()
{
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

bool uwbPacketAvailable()
{
    return packet_ready;
}

void uwbGetPacket(uint8_t *buffer, uint16_t *len)
{
    memcpy(buffer, rx_buffer, rx_len);
    *len = rx_len;
    packet_ready = false;
}

uint64_t uwbGetRxTimestamp()
{
    uint8_t ts_tab[5];
    uint64_t ts = 0;

    dwt_readrxtimestamp(ts_tab);

    for (int i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }

    return ts;
}

void uwbSendFrame(uint8_t *data, uint16_t len)
{
    dwt_writetxdata(len, data, 0);
    dwt_writetxfctrl(len, 0, 1);
    dwt_starttx(DWT_START_TX_IMMEDIATE);
}