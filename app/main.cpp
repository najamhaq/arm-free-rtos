#include <stdint.h>




extern "C" void SysTick_Handler(void)
{
}



void setup()
{
}

void super_loop()
{
  while (true)
  {
  /* nothing */
  }
}

int main(void) {
  // if we had memory management , we could use constructor
  setup();
  super_loop();
  return 0;
}