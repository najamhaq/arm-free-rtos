//
// Created by najamhaq on 2026-01-21.
//

#include "Debouncer.h"

Debouncer::Debouncer(uint32_t debounce_ms) {
  debounce_delay_ms = debounce_ms;
  // initial states
  btnA = ButtonTracker{.state = DebounceState::Idle, .last_transition_ms = 0};
  btnB = ButtonTracker{.state = DebounceState::Idle, .last_transition_ms = 0};
};

Events Debouncer::update(ButtonsRaw raw, uint32_t event_time_ms) {
  Events result;
  result.count = 0;
  step(ButtonId::A, raw.a, btnA, event_time_ms, result);
  step(ButtonId::B, raw.b, btnB, event_time_ms, result);
  return result;
}
void Debouncer::step(ButtonId button_id, bool is_down, ButtonTracker& button,
                     uint32_t event_time_in_ms, Events& out) {

  const uint32_t elapsed_ms = event_time_in_ms - button.last_transition_ms;
  const bool debounced = (elapsed_ms >= debounce_delay_ms);

  switch (button.state) {
  case DebounceState::PressDebounce: {
    if (!is_down) {
      button.state = DebounceState::Idle;
      button.last_transition_ms = event_time_in_ms;
      break;
    }

    // button is down
    if (debounced) {
      button.state = DebounceState::PressConfirmed;
      out.push(button_id, Edge::Press, event_time_in_ms);
    }
    break;
  }

  case DebounceState::PressConfirmed:
  case DebounceState::Held: {
    if (is_down) {
      button.state = DebounceState::Held; // stay in "held" path
    } else {
      button.state = DebounceState::ReleaseDebounce;
      button.last_transition_ms = event_time_in_ms;
    }
    break;
  }

  case DebounceState::ReleaseDebounce: {
    if (is_down) {
      button.state = DebounceState::PressDebounce;
      button.last_transition_ms = event_time_in_ms;
      break;
    }

    if (debounced) {
      button.state = DebounceState::ReleaseConfirmed;
      button.last_transition_ms = event_time_in_ms;
      out.push(button_id, Edge::Release, event_time_in_ms);
    }
    break;
  }

  case DebounceState::ReleaseConfirmed: {
    if (is_down) {
      button.state = DebounceState::PressDebounce;
    } else {
      button.state = DebounceState::Idle;
    }
    break;
  }

  case DebounceState::Idle:
  default: {
    button.last_transition_ms = event_time_in_ms;
    if (is_down) {
      button.state = DebounceState::PressDebounce;
    } else {
      button.state = DebounceState::Idle; // explicit “stay”
    }
    break;
  }
  }
}
