#include "ethernet_manager.h"
#include <ETH.h>

void initEthernet()
{
    ETH.begin();
    while (!ETH.linkUp())
    {
        delay(100);
    }
}