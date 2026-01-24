//
// Created by mnajm on 2026-01-22.
//

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "app_queues.h"
#include "debouncer.h"
#include "button.h"

static uint32_t now_ms() {
  // FreeRTOS tick count → milliseconds
  // Note: portTICK_PERIOD_MS exists in FreeRTOS.
  return static_cast<uint32_t>(xTaskGetTickCount()) * portTICK_PERIOD_MS;
}

extern "C" void ButtonTask(void* /*arg*/) {
  Debouncer debouncer(/*debounce_ms=*/30);

  const TickType_t period = pdMS_TO_TICKS(10);
  TickType_t lastWake = xTaskGetTickCount();

  for (;;) {
    ButtonsRaw raw = buttons_read_raw();
    uint32_t t = now_ms();

    auto events = debouncer.update(raw, t);

    for (int i = 0; i < events.count; i++) {
      InputEvent e = events.items[i];
      // Non-blocking send; change to a small timeout if you prefer reliability
      (void)xQueueSend(g_inputQueue, &e, 0);
    }

    vTaskDelayUntil(&lastWake, period);
  }
}
