//
// Created by mnajm on 2026-01-19.
//

#ifndef MB2_FREERTOS_MB2_BOARD_H
#define MB2_FREERTOS_MB2_BOARD_H

#include <stdint.h>

#define GPIO_0_BASE 0x50000000UL
#define GPIO_1_BASE 0x50000300UL

#define OUTSET (*(volatile uint32_t*)(GPIO_0_BASE + 0x508))
#define OUTCLR (*(volatile uint32_t*)(GPIO_0_BASE + 0x50C))

#define P0_DIRSET (*(volatile uint32_t*)(GPIO_0_BASE + 0x518))
#define P1_DIRSET (*(volatile uint32_t*)(GPIO_1_BASE + 0x518)) // 0x50000300 + 0x518 = 0x50000818

#define P0_OUTSET (*(volatile uint32_t*)(GPIO_0_BASE + 0x508))
#define P0_OUTCLR (*(volatile uint32_t*)(GPIO_0_BASE + 0x50C))
#define P1_OUTSET (*(volatile uint32_t*)(GPIO_1_BASE + 0x508))
#define P1_OUTCLR (*(volatile uint32_t*)(GPIO_1_BASE + 0x50C))

#define P0_DIRCLR (*(volatile uint32_t*)(GPIO_0_BASE + 0x51C))

#define P0_IN (*(volatile uint32_t*)(GPIO_0_BASE + 0x510))
#define P0_DIRCLR (*(volatile uint32_t*)(GPIO_0_BASE + 0x51C))
#define P0_PIN_CNF(n) (*(volatile uint32_t*)(GPIO_0_BASE + 0x700 + 4u * (n)))

#define BTN_A_PIN 14u
#define BTN_B_PIN 23u

// PIN_CNF bits (nRF style):
// DIR=0 (input), PULL=3 (pull-up) in many nRF headers,
// but since you don't have enums, we'll set by bit positions:

#define PIN_CNF_DIR_Pos 0u
#define PIN_CNF_PULL_Pos 2u
#define PIN_CNF_INPUT_Pos 1u

#define PIN_CNF_DIR_Input (0u)
#define PIN_CNF_INPUT_Connect (0u)
#define PIN_CNF_PULL_Pullup (3u)

static inline uint32_t BIT(uint32_t pin) { return (1u << (pin)); }

// Some pins are not one the sam eport. Define a struct to represent a GPIO pin.
typedef struct {
  uint8_t port; // 0 or 1
  uint8_t pin;  // 0..31 (port0) or 0..15 (port1)
} gpio_t;

// Following are the GPIO pins for the LED matrix on micro:bit v2

#endif // MB2_FREERTOS_MB2_BOARD_H