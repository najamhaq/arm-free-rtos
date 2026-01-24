//
// Created by mnajm on 2026-01-24.
//

#ifndef MICROBIT_FREERTOS_SRC_PLATFORM_INCLUDE_BUTTON_HW_H
#define MICROBIT_FREERTOS_SRC_PLATFORM_INCLUDE_BUTTON_HW_H

#include "microbit_ver_2.h"

static inline bool button_a_pressed() { return ((P0_IN & (1u << BTN_A_PIN)) == 0u); };
static inline bool button_b_pressed() { return ((P0_IN & (1u << BTN_B_PIN)) == 0u); };

#endif // MICROBIT_FREERTOS_SRC_PLATFORM_INCLUDE_BUTTON_HW_H
