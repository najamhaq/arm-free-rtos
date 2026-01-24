//
// Created by najamhaq on 2026-01-21.
//

#ifndef MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H
#define MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H

#pragma once

#include "button.h"
#include "input_type.h"
#include <cstdint>

struct Events {
  InputEvent items[2];
  uint8_t count{0};

  void push(ButtonId btn, Edge e, uint32_t currentTimeMilliSec) {
    if (count < 2) {
      items[count++] = InputEvent{btn, e, currentTimeMilliSec};
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
  explicit Debouncer(uint32_t debounceDelayInMilliSec = 30);

  // Called at fixed tick intervals. Returns up to 2 events (A/B).
  Events update(ButtonsRaw raw, uint32_t eventTimeMilliSec);

private:
  // Advances one button's state machine and appends any event into 'out'.
  void step(ButtonId id, bool isDown, ButtonTracker& button, uint32_t eventTimeMilliSec,
            Events& out) const;

private:
  uint32_t debounceDelayMilliSec{30};
  ButtonTracker buttonA;
  ButtonTracker buttonB;
};

#endif // MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H
