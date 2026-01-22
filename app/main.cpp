extern "C" {
#include "FreeRTOS.h"
#include "mb2_board.h"
#include "task.h"
}

#include "Debouncer.h"
#include "LEDMatrix.h"
void setup() { LEDMatrix::init(); }

static void BlinkTask(void*) {
  for (;;) {
    __asm volatile("nop");
    vTaskDelay(pdMS_TO_TICKS(500));
    LEDMatrix::led_on(3, 3);
    vTaskDelay(pdMS_TO_TICKS(500));
    LEDMatrix::led_off(3, 3);
  }
}

// static void ButtonTask(void*) {
//   auto * debouncer = new Debouncer(20);
//   for (;;) {
//     __asm volatile("nop");
////    debouncer->update();
//    vTaskDelay(pdMS_TO_TICKS(20));
//  }
//}

extern "C" int main(void) {
  setup();
  xTaskCreate(BlinkTask, "blink", 128, nullptr, 1, nullptr);
  //  xTaskCreate(ButtonTask, "button", 128, nullptr, 1, nullptr);
  vTaskStartScheduler();

  // If we get here, scheduler failed (heap/stack)
  for (;;) {
    __asm volatile("nop");
  }
}
