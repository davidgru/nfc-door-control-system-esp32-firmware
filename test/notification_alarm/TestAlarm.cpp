
#include <Arduino.h>
#include <unity.h>

#include <notification.hpp>


DC::Alarm _alarm(ALARM_PIN, ALARM_DURATION_MILLIS);


void tearDown() {
    delay(ALARM_DURATION_MILLIS);
}


void testAlarm() {
    _alarm.onEvent(DC::DoorbellEvent{});
    TEST_ASSERT_EQUAL(HIGH, digitalRead(ALARM_PIN));
}


void testAlarmCancelled() {
    _alarm.onEvent(DC::DoorbellEvent{});
    _alarm.onEvent(DC::DoorOpenEvent{});
    TEST_ASSERT_EQUAL(LOW, digitalRead(ALARM_PIN));
}


void testAlarmOnAlready() {
    _alarm.onEvent(DC::DoorbellEvent{});
    _alarm.onEvent(DC::DoorbellEvent{});
    TEST_ASSERT_EQUAL(HIGH, digitalRead(ALARM_PIN));
}


void testAlarmDuration() {
    _alarm.onEvent(DC::DoorbellEvent{});
    delay(ALARM_DURATION_MILLIS - 100);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(ALARM_PIN));
    delay(200);
    TEST_ASSERT_EQUAL(LOW, digitalRead(ALARM_PIN));
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testAlarm);
    RUN_TEST(testAlarmCancelled);
    RUN_TEST(testAlarmDuration);
    RUN_TEST(testAlarmOnAlready);

    UNITY_END();
}


void loop() {

}
