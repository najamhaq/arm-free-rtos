//
// Created by najamhaq on 2026-01-24.
//

#include "uart_hw.h"
#include "microbit_ver_2.h" // for pin defs
#include "nrf.h"

void uart_hw_init(uint32_t baudrate) {
  NRF_UARTE0->ENABLE = UARTE_ENABLE_ENABLE_Disabled;

  NRF_UARTE0->PSEL.TXD = MB2_UART_TX_PIN;
  NRF_UARTE0->PSEL.RXD = MB2_UART_RX_PIN;

  NRF_UARTE0->BAUDRATE = baudrate;
  NRF_UARTE0->CONFIG = 0;

  NRF_UARTE0->ENABLE = UARTE_ENABLE_ENABLE_Enabled;
  NRF_UARTE0->TASKS_STARTTX = 1;
}

void uart_hw_write_byte(uint8_t byte) {
  NRF_UARTE0->TXD.PTR = (uint32_t)&byte;
  NRF_UARTE0->TXD.MAXCNT = 1;

  NRF_UARTE0->EVENTS_ENDTX = 0;
  NRF_UARTE0->TASKS_STARTTX = 1;

  while (NRF_UARTE0->EVENTS_ENDTX == 0) {
    /* busy wait */
  }
}
