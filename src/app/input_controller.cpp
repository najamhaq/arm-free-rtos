//
// Created by najamhaq on 2026-01-22.
//

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "app_queues.h"
#include "led_matrix.h"
#include "lock.h"
#include "debouncer.h"
#include "ui_lock.h"

static void onA() {
  Lock lk(g_ledMutex);
  LEDMatrix::led_on(0, 0);
  vTaskDelay(1000);
  LEDMatrix::led_off(0, 0);
}
static void onB() {
  Lock lk(g_ledMutex);
  LEDMatrix::led_on(4, 4);
  vTaskDelay(1000);
  LEDMatrix::led_off(4, 4);
}

extern "C" void InputController(void*) {

  InputEvent e{};
  for (;;) {
    if (xQueueReceive(g_inputQueue, &e, portMAX_DELAY) == pdTRUE) {
      // update current down-state
      switch (e.edge) {
      case Edge::Press:
        if (e.button == ButtonId::A)
          onA();
        else if (e.button == ButtonId::B)
          onB();
        break;

      case Edge::Release:
        break;
      }
    }
  }
}