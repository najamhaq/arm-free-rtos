extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "mb2_board.h"
#include "LEDMatrix.h"
}

void setup() {
  LEDMatrix::init();
}

static void BlinkTask(void *) {
  for (;;) {
    __asm volatile ("nop");
    vTaskDelay(pdMS_TO_TICKS(500));
    LEDMatrix::led_on(3, 3);
    vTaskDelay(pdMS_TO_TICKS(500));
    LEDMatrix::led_off(3, 3);
  }
}

extern "C" int main(void) {
  setup();
  xTaskCreate(BlinkTask, "blink", 128, nullptr, 1, nullptr);
  vTaskStartScheduler();

  // If we get here, scheduler failed (heap/stack)
  for (;;) {
    __asm volatile ("nop");
  }
}
