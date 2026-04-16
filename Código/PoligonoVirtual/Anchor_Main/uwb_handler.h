#ifndef UWB_HANDLER_H
#define UWB_HANDLER_H

#include <Arduino.h>

// Captura las interrupciones del DW3000 y las guarda en el buffer circular
void uwb_handler_capture();

// Extrae los datos del buffer circular y los delega al motor UDP
void uwb_handler_process_buffer();

#endif // UWB_HANDLER_H