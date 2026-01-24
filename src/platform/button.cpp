//
// Created by najamhaq on 2026-01-22.
//

#include "button.h" //  # probably not needed .
#include "button_hw.h"
#include "microbit_ver_2.h" // define hardware level pins

void buttons_init() {
  // Make pins inputs
  P0_DIRCLR = (1u << BTN_A_PIN) | (1u << BTN_B_PIN);

  // Enable pull-ups (so released reads as 1)
  P0_PIN_CNF(BTN_A_PIN) = (PIN_CNF_DIR_Input << PIN_CNF_DIR_Pos) |
                          (PIN_CNF_INPUT_Connect << PIN_CNF_INPUT_Pos) |
                          (PIN_CNF_PULL_Pullup << PIN_CNF_PULL_Pos);

  P0_PIN_CNF(BTN_B_PIN) = (PIN_CNF_DIR_Input << PIN_CNF_DIR_Pos) |
                          (PIN_CNF_INPUT_Connect << PIN_CNF_INPUT_Pos) |
                          (PIN_CNF_PULL_Pullup << PIN_CNF_PULL_Pos);
}
