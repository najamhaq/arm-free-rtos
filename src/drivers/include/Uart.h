//
// Created by mnajm on 2026-01-24.
//

#ifndef MICROBIT_FREERTOS_SRC_DRIVERS_INCLUDE_UART_H
#define MICROBIT_FREERTOS_SRC_DRIVERS_INCLUDE_UART_H

#include <cstdint>

class Uart {
public:
  void init(uint32_t baudrate);
  void writeByte(uint8_t byte);
  void write(const char* str);
};

#endif // MICROBIT_FREERTOS_SRC_DRIVERS_INCLUDE_UART_H
