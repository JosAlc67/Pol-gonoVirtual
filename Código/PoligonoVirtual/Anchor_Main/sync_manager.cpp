#include "sync_manager.h"
#include "config.h"
#include "uwb_driver.h"

static int64_t sync_offset = 0;
static uint32_t last_sync_time = 0;

void initSync()
{
    sync_offset = 0;
    last_sync_time = millis();
}

void processSync()
{
#if ANCHOR_IS_MASTER

    // Enviar SYNC cada 100 ms
    if (millis() - last_sync_time > 100)
    {
        uint8_t sync_frame[4] = {0xA5, 0x5A, 0x01, 0x00};
        uwbSendFrame(sync_frame, sizeof(sync_frame));
        last_sync_time = millis();
    }

#else

    // En slaves se ajustaría offset aquí
    // (implementación avanzada después)

#endif
}

int64_t getSyncOffset()
{
    return sync_offset;
}