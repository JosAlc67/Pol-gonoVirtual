#include "ring_buffer.h"

static UWBFrame buffer[RING_SIZE];
static volatile uint16_t head = 0;
static volatile uint16_t tail = 0;

void ringInit()
{
    head = tail = 0;
}

bool ringPush(uint8_t *data, uint16_t len, uint64_t ts)
{
    uint16_t next = (head + 1) % RING_SIZE;

    if (next == tail)
        return false;

    memcpy(buffer[head].data, data, len);
    buffer[head].len = len;
    buffer[head].timestamp = ts;

    head = next;
    return true;
}

bool ringPop(UWBFrame *frame)
{
    if (tail == head)
        return false;

    *frame = buffer[tail];
    tail = (tail + 1) % RING_SIZE;

    return true;
}