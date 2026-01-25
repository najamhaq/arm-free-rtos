//
// Created by najamhaq  on 2026-01-24.
//

#include "uart.h"
#include "uart_hw.h"

void Uart::init(uint32_t baudrate) { uart_hw_init(baudrate); }

void Uart::writeByte(uint8_t byte) { uart_hw_write_byte(byte); }

void Uart::write(const char* str) {
  while (*str) {
    writeByte(static_cast<uint8_t>(*str++));
  }
}
