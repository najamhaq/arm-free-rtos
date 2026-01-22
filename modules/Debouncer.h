//
// Created by najamhaq on 2026-01-21.
//

#ifndef MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H
#define MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H

#pragma once

#include "InputEvent.h"
#include <cstdint>

// If you later want to keep the older A_Press/B_Press style,
// you can always convert (ButtonId, Edge) -> InputEventType at the boundary.

struct Events {
  InputEvent items[2];
  uint8_t count{0};

  void push(ButtonId btn, Edge e, uint32_t t_ms) {
    if (count < 2) {
      items[count++] = InputEvent{btn, e, t_ms};
    }
  }
};

enum class DebounceState : uint8_t {
  Idle,            // stable up
  PressDebounce,   // saw down edge, waiting for debounce
  PressConfirmed,  // press confirmed (event emitted)
  Held,            // still held down
  ReleaseDebounce, // saw up edge, waiting for debounce
  ReleaseConfirmed // release confirmed (event emitted)
};

struct ButtonTracker {
  DebounceState state{DebounceState::Idle};
  uint32_t last_transition_ms{0};
};

class Debouncer {
public:
  explicit Debouncer(uint32_t debounce_ms = 30);

  // Called at fixed tick intervals. Returns up to 2 events (A/B).
  Events update(ButtonsRaw raw, uint32_t event_time_ms);

private:
  // Advances one button's state machine and appends any event into 'out'.
  void step(ButtonId id, bool is_down, ButtonTracker& button, uint32_t now_ms, Events& out);

private:
  uint32_t debounce_delay_ms{30};
  ButtonTracker btnA;
  ButtonTracker btnB;
};

#endif // MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H
