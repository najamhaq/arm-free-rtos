//
// Created by najamhaq on 2026-01-20.
//

#include "debouncer.h"
#include "unity.h"
#include <stdint.h>
/*

“”




 */
/* Optional hooks (Unity will call these around each test) */
void setUp(void) {}
void tearDown(void) {}

/* ---- Tests ---- */
void test_button_pressed_for_less_than_debounce(void) {
  ButtonsRaw button_up{false, false};
  ButtonsRaw button_down{true, false};

  Debouncer debouncer(20);
  //  ButtonInput btn_A = {};

  /* Assure that Button Press raises an event */
  Events events = debouncer.update(button_up, 0);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_up, 10);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 20);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_up, 30);
  TEST_ASSERT_TRUE(events.count == 0);
}

/* ---- Tests ---- */
void test_short_press_for_20ms_generates_no_event(void) {
  ButtonsRaw button_up{false, false};
  ButtonsRaw button_down{true, false};

  Debouncer debouncer(20);
  //  ButtonInput btn_A = {};

  /* Assure that Button Press raises an event */
  Events events = debouncer.update(button_up, 0);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 10);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 20);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_up, 30);
  TEST_ASSERT_TRUE(events.count == 0);
}

void test_stable_press_for_30ms_generates_A_Press_exactly_once(void) {
  ButtonsRaw button_up{false, false};
  ButtonsRaw button_down{true, false};

  Debouncer debouncer(20);
  //  ButtonInput btn_A = {};

  /* Assure that Button Press raises an event */
  Events events = debouncer.update(button_up, 0);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 10);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 20);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 30);
  TEST_ASSERT_TRUE(events.count != 0);
}

void test_stable_press_for_too_long_generates_A_Press_exactly_once(void) {
  ButtonsRaw button_up{false, false};
  ButtonsRaw button_down{true, false};

  Debouncer debouncer(20);
  //  ButtonInput btn_A = {};

  /* Assure that Button Press raises an event */
  Events events = debouncer.update(button_up, 0);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 10);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 20);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 30);
  TEST_ASSERT_TRUE(events.count != 0);
  events = debouncer.update(button_down, 40);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 50);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 60);
  TEST_ASSERT_TRUE(events.count == 0);
}

void test_A_and_B_together_produce_both_events_correctly(void) {
  ButtonsRaw button_up{false, false};
  ButtonsRaw button_down{true, true}; /// <==== both button pressed

  Debouncer debouncer(20);
  //  ButtonInput btn_A = {};

  /* Assure that Button Press raises an event */
  Events events = debouncer.update(button_up, 0);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 10);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 20);
  TEST_ASSERT_TRUE(events.count == 0);
  events = debouncer.update(button_down, 30);
  TEST_ASSERT_TRUE(events.count == 2);
}

void test_button_debounce(void) {
  /* Assure that too frequent button press do not raise multipole events */
  TEST_ASSERT_TRUE(1);
}

int main(void) {
  UNITY_BEGIN();

  /* Register tests here */
  RUN_TEST(test_button_pressed_for_less_than_debounce);
  RUN_TEST(test_stable_press_for_30ms_generates_A_Press_exactly_once);
  RUN_TEST(test_stable_press_for_too_long_generates_A_Press_exactly_once);
  RUN_TEST(test_short_press_for_20ms_generates_no_event);
  RUN_TEST(test_A_and_B_together_produce_both_events_correctly);

  return UNITY_END();
}
