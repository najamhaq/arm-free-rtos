//
// Created by najamhaq on 2026-01-21.
//

#include "debouncer.h"

Debouncer::Debouncer(uint32_t debounceDelayInMilliSec) {
  debounceDelayMilliSec = debounceDelayInMilliSec;
  // initial states
  buttonA = ButtonTracker{.state = DebounceState::Idle, .last_transition_ms = 0};
  buttonB = ButtonTracker{.state = DebounceState::Idle, .last_transition_ms = 0};
};

Events Debouncer::update(ButtonsRaw raw, uint32_t eventTimeMilliSec) {
  Events result;
  result.count = 0;
  step(ButtonId::A, raw.a, buttonA, eventTimeMilliSec, result);
  step(ButtonId::B, raw.b, buttonB, eventTimeMilliSec, result);
  return result;
}
void Debouncer::step(ButtonId buttonId, bool isDown, ButtonTracker& button,
                     uint32_t eventTimeMilliSec, Events& out) const {

  const uint32_t elapsedTimeMilliSec = eventTimeMilliSec - button.last_transition_ms;
  const bool debounced = (elapsedTimeMilliSec >= debounceDelayMilliSec);

  switch (button.state) {
  case DebounceState::PressDebounce: {
    if (!isDown) {
      button.state = DebounceState::Idle;
      button.last_transition_ms = eventTimeMilliSec;
      break;
    }

    // button is down
    if (debounced) {
      button.state = DebounceState::PressConfirmed;
      out.push(buttonId, Edge::Press, eventTimeMilliSec);
    }
    break;
  }

  case DebounceState::PressConfirmed:
  case DebounceState::Held: {
    if (isDown) {
      button.state = DebounceState::Held; // stay in "held" path
    } else {
      button.state = DebounceState::ReleaseDebounce;
      button.last_transition_ms = eventTimeMilliSec;
    }
    break;
  }

  case DebounceState::ReleaseDebounce: {
    if (isDown) {
      button.state = DebounceState::PressDebounce;
      button.last_transition_ms = eventTimeMilliSec;
      break;
    }

    if (debounced) {
      button.state = DebounceState::ReleaseConfirmed;
      button.last_transition_ms = eventTimeMilliSec;
      out.push(buttonId, Edge::Release, eventTimeMilliSec);
    }
    break;
  }

  case DebounceState::ReleaseConfirmed: {
    if (isDown) {
      button.state = DebounceState::PressDebounce;
    } else {
      button.state = DebounceState::Idle;
    }
    break;
  }

  case DebounceState::Idle:
  default: {
    button.last_transition_ms = eventTimeMilliSec;
    if (isDown) {
      button.state = DebounceState::PressDebounce;
    } else {
      button.state = DebounceState::Idle; // explicit “stay”
    }
    break;
  }
  }
}
