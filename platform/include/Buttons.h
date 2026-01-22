//
// Created by mnajm on 2026-01-22.
//

#ifndef MICROBIT_FREERTOS_PLATFORM_INCLUDE_BUTTONS_H
#define MICROBIT_FREERTOS_PLATFORM_INCLUDE_BUTTONS_H
#pragma once
#include "InputEvent.h" // for ButtonsRaw

ButtonsRaw buttons_read_raw(); // returns true=pressed
void buttons_init();

#endif // MICROBIT_FREERTOS_PLATFORM_INCLUDE_BUTTONS_H
