extern "C" {
#include "FreeRTOS.h"
#include "mb2_board.h"
#include "task.h"
}

#include "AppQueues.h"
#include "Buttons.h"
#include "Debouncer.h"
#include "LEDMatrix.h"
#include "Lock.h"
#include "UILock.h"

// forward declare task entry (C linkage if needed)
extern "C" void ButtonTask(void*);
extern "C" void InputController(void*);

static void BlinkTask(void*) {
  for (;;) {
    __asm volatile("nop");
    vTaskDelay(pdMS_TO_TICKS(500));
    Lock lk(g_ledMutex);
    LEDMatrix::led_on(3, 3);
    vTaskDelay(pdMS_TO_TICKS(500));
    LEDMatrix::led_off(3, 3);
  }
}

void rtos_setup() {
  g_ledMutex = xSemaphoreCreateMutex();
  configASSERT(g_ledMutex != nullptr);
  g_inputQueue = xQueueCreate(/*length=*/16, /*item_size=*/sizeof(InputEvent));
  configASSERT(g_inputQueue != nullptr);

  BaseType_t ok = xTaskCreate(ButtonTask, "Button",
                              /*stackWords=*/256, nullptr,
                              /*priority=*/tskIDLE_PRIORITY + 2, nullptr);

  configASSERT(ok == pdPASS);

  BaseType_t ok2 =
      xTaskCreate(InputController, "InputProc", 256, nullptr, tskIDLE_PRIORITY + 2, nullptr);

  configASSERT(ok2 == pdPASS);

  ok2 = xTaskCreate(BlinkTask, "blink", 128, nullptr, 1, nullptr);
  configASSERT(ok2 == pdPASS);
}

void hardware_setup() {
  LEDMatrix::init();
  buttons_init();
}

extern "C" int main(void) {
  hardware_setup();
  rtos_setup();
  vTaskStartScheduler();

  // If we get here, scheduler failed (heap/stack)
  for (;;) {
    __asm volatile("nop");
  }
}
