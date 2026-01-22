//
// Created by najamhaq on 2026-01-21.
//

#ifndef MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H
#define MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H

#include "cstdint"
#include <array>

struct ButtonsRaw {
  bool a;
  bool b;
};
enum class InputEventType { A_Press, A_Release, B_Press, B_Release, A_None, B_None };

enum class states { STATE_0, STATE_1, STATE_2, STATE_3, STATE_4, STATE_5 };

struct btn_state {
  states state;
  uint32_t time_ms;
};

struct InputEvent {
  InputEventType type;
  uint32_t t_ms;
};

struct Events {
  InputEvent* events[2];
  int count;
};

class Debouncer {
private:
  uint32_t debounce_delay;
  btn_state a_state;
  btn_state b_state;
  uint32_t a_ms;
  uint32_t b_ms;

public:
  explicit Debouncer(uint32_t debounce_ms = 30);

  // called at fixed tick intervals
  Events update(ButtonsRaw raw, uint32_t now_ms);
  InputEvent* getEvent(bool buttonDown, btn_state& btn_state, uint32_t now_ms);
};

#endif // MICROBIT_FREERTOS_APP_BUTTOONDEBOUNCER_H
