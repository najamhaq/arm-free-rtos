//
// Created by najamhaq on 2026-01-22.
//

#include "button.h" //  # probably not needed .
#include "button_hw.h"
#include "microbit_ver_2.h" // define hardware level pins

void buttons_init() {
  // Make pins inputs
  NRF_P0->DIRCLR = (1u << MB2_BTN_A_PIN) | (1u << MB2_BTN_B_PIN);

  // Enable pull-ups (so released reads as 1)

  NRF_P0->PIN_CNF[MB2_BTN_A_PIN] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);

  NRF_P0->PIN_CNF[MB2_BTN_B_PIN] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
}
