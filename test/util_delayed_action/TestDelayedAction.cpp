
#include <Arduino.h>
#include <unity.h>


#include "DelayedAction.hpp"


#define TEST_DELAY 500 // 0.5s
#define TEST_CALLBACK_DURATION 500 // 0.5s

#define CALLBACK_ARG_INITIAL_VALUE 5
#define CALLBACK_ARG_UPDATED_VALUE 6

int callbackArg = CALLBACK_ARG_INITIAL_VALUE;

void callback(int* arg) {
    delay(TEST_CALLBACK_DURATION / 2);
    *arg = CALLBACK_ARG_UPDATED_VALUE;
    delay(TEST_CALLBACK_DURATION / 2);
}

DC::DelayedAction<int> delayedAction(TEST_DELAY, callback, &callbackArg);


void setUp() {
    callbackArg = CALLBACK_ARG_INITIAL_VALUE;
}


void tearDown() {
    delay(TEST_DELAY + TEST_CALLBACK_DURATION);
}


void testExecution() {
    delayedAction.schedule();
    delay(TEST_DELAY + TEST_CALLBACK_DURATION);
    TEST_ASSERT_EQUAL_INT(CALLBACK_ARG_UPDATED_VALUE, callbackArg);
}


void testTiming() {
    delayedAction.schedule();
    delay(TEST_DELAY);
    TEST_ASSERT_EQUAL_INT(CALLBACK_ARG_INITIAL_VALUE, callbackArg);
    delay(TEST_CALLBACK_DURATION);
    TEST_ASSERT_EQUAL_INT(CALLBACK_ARG_UPDATED_VALUE, callbackArg);
}


void testCancelWhileWait() {
    delayedAction.schedule();
    delay(TEST_DELAY / 2);
    delayedAction.cancel();
    delay(TEST_DELAY / 2 + TEST_CALLBACK_DURATION);
    TEST_ASSERT_EQUAL_INT(CALLBACK_ARG_INITIAL_VALUE, callbackArg);
}


void testCancelWhileExecuting() {
    delayedAction.schedule();
    delay(TEST_DELAY + TEST_CALLBACK_DURATION / 2);
    delayedAction.cancel();
    delay(TEST_CALLBACK_DURATION / 2);
    TEST_ASSERT_EQUAL_INT(CALLBACK_ARG_UPDATED_VALUE, callbackArg);
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testExecution);

    RUN_TEST(testTiming);

    RUN_TEST(testCancelWhileWait);

    RUN_TEST(testCancelWhileExecuting);

    UNITY_END();
}

void loop() {
    // No need.
}
