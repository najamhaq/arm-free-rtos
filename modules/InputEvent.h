//
// Created by mnajm on 2026-01-22.
//

#ifndef MICROBIT_FREERTOS_MODULES_INPUTEVENT_H
#define MICROBIT_FREERTOS_MODULES_INPUTEVENT_H

#pragma once
#include <cstdint>

enum class ButtonId : uint8_t { A, B };
enum class Edge : uint8_t { Press, Release };

struct ButtonsRaw {
  bool a{false}; // true = pressed (recommended convention)
  bool b{false}; // true = pressed
};

struct InputEvent {
  ButtonId button;
  Edge edge;
  uint32_t t_ms;
};

#endif // MICROBIT_FREERTOS_MODULES_INPUTEVENT_H
