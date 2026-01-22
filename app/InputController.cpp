//
// Created by najamhaq on 2026-01-22.
//

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "LEDMatrix.h"
#include "Lock.h"
#include "UILock.h"
#include "app/include/AppQueues.h"
#include "modules/InputEvent.h"

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