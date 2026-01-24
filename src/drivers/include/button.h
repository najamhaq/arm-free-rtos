//
// Created by najamhaq on 2026-01-22.
//

#ifndef MICROBIT_FREERTOS_PLATFORM_INCLUDE_BUTTON_H
#define MICROBIT_FREERTOS_PLATFORM_INCLUDE_BUTTON_H

#include "input_type.h"
#include <cstdint>

enum class ButtonId : uint8_t { A, B };
enum class Edge : uint8_t { Press, Release };

struct InputEvent {
  ButtonId button;
  Edge edge;
  uint32_t event_time;
};

ButtonsRaw buttons_read_raw(); // returns true=pressed
void buttons_init();

#endif // MICROBIT_FREERTOS_PLATFORM_INCLUDE_BUTTON_H
