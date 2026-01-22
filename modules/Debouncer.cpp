//
// Created by mnajm on 2026-01-21.
//

#include "Debouncer.h"

Debouncer::Debouncer(uint32_t debounce_ms) {
  debounce_delay = debounce_ms;
  // initial states
  a_state = btn_state{.state = states::STATE_0, .time_ms = 0};
  b_state = btn_state{.state = states::STATE_0, .time_ms = 0};
};

Events Debouncer::update(ButtonsRaw raw, uint32_t now_ms) {
  Events result;
  result.count = 0;
  InputEvent* btnAEvent = nullptr;
  InputEvent* btnBEvent = nullptr;
  btnAEvent = getEvent(raw.a, a_state, now_ms);
  btnBEvent = getEvent(raw.b, b_state, now_ms);
  if (btnAEvent || btnBEvent) {
    // check if btnAEvent is not null
    if (btnAEvent)
      result.events[result.count++] = btnAEvent;
    if (btnBEvent)
      result.events[result.count++] = btnBEvent;
  }
  return result;
}
InputEvent* Debouncer::getEvent(const bool btnDown, btn_state& btn_state, uint32_t now_ms) {
  InputEvent* btnAEvent = nullptr;
  uint32_t time_diff = now_ms - btn_state.time_ms;
  bool time_over_debounce = time_diff >= debounce_delay;

  switch (btn_state.state) {
  case states::STATE_1:
    if (!btnDown) {
      btn_state.state = states::STATE_0;
      btn_state.time_ms = now_ms;
      return nullptr;
    }
    // button is down
    if (time_over_debounce) {
      btn_state.state = states::STATE_2;
      // this is where we emit an event ..
      btnAEvent = new InputEvent{InputEventType::A_Press, now_ms};
      return btnAEvent;
    }
    // stay in state 1
    break;
  case states::STATE_2:
    if (btnDown) {
      btn_state.state = states::STATE_3;
      return nullptr;
    }
    btn_state.state = states::STATE_4;
    btn_state.time_ms = now_ms;
    return nullptr;
    break;
  case states::STATE_3:
    if (btnDown) {
      btn_state.state = states::STATE_3;
      return nullptr;
    }
    btn_state.state = states::STATE_4;
    btn_state.time_ms = now_ms;
    return nullptr;
    break;
  case states::STATE_4:
    if (btnDown) {
      btn_state.state = states::STATE_1;
      btn_state.time_ms = now_ms;
      return nullptr;
    }
    if (time_over_debounce) {
      // emit 0 here
      btn_state.state = states::STATE_5;
      btn_state.time_ms = now_ms;
      btnAEvent = new InputEvent{InputEventType::A_Release, now_ms};
      return btnAEvent;
    }
    // stay in state 4
    break;
  case states::STATE_5:
    if (btnDown) {
      btn_state.state = states::STATE_1;
      return nullptr;
    }

    btn_state.state = states::STATE_0;
    return nullptr;
    break;

  case states::STATE_0:
  default:
    btn_state.time_ms = now_ms;
    if (!btnDown) {
      // staty in state 0;
      btn_state.state = states::STATE_0;
      return nullptr;
    }
    btn_state.state = states::STATE_1;
    return nullptr;
  }
  // should never be here
  return nullptr;
}