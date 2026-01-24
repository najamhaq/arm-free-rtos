//
// Created by najamhaq on 2026-01-24.
//

#include "button.h"
#include "button_hw.h"

ButtonsRaw buttons_read_raw() {
  ButtonsRaw r{};
  // Example ONLY:
  r.a = button_a_pressed(); // active-low → pressed=true
  r.b = button_b_pressed();
  return r;
}
