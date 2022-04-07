
#include <Arduino.h>
#include <unity.h>


#include <door.hpp>




#define DOOR_TEST_PIN DOOR_OPEN_PIN
#define DOOR_TEST_DURATION DOOR_OPEN_DURATION_MILLIS


DC::DoorOpener doorOpener(DOOR_TEST_PIN, DOOR_TEST_DURATION);


void setUp() {

}


void tearDown() {
    // Make sure that LED is off after test.
    delay(DOOR_TEST_DURATION);
    digitalWrite(DOOR_TEST_PIN, LOW);
}


void testDefault() {
    // LED should be off by default.
    TEST_ASSERT_EQUAL(LOW, digitalRead(DOOR_TEST_PIN));
}


void testUnlock() {
    doorOpener.openDoor();
    TEST_ASSERT_EQUAL(HIGH, digitalRead(DOOR_TEST_PIN));
}


void testWhileUnlocked() {
    doorOpener.openDoor();
    TEST_ASSERT_EQUAL(HIGH, digitalRead(DOOR_TEST_PIN));
    delay(DOOR_TEST_DURATION / 2);
    doorOpener.openDoor(); // Should not extend open time.
    delay(DOOR_TEST_DURATION / 2);
    TEST_ASSERT_EQUAL(LOW, digitalRead(DOOR_TEST_PIN));
}


void testUnlockDuration() {
    doorOpener.openDoor();
    delay(DOOR_TEST_DURATION - 100); // Give 0.1s error.
    TEST_ASSERT_EQUAL(HIGH, digitalRead(DOOR_TEST_PIN));
    delay(200); // Again give 0.1s after.
    TEST_ASSERT_EQUAL(LOW, digitalRead(DOOR_TEST_PIN));
}


class EventListenerImpl : public DC::EventListener<DC::DoorOpenEvent> {
public:
    void onEvent(const DC::DoorOpenEvent& event) override {
        this->i = 1;
    }

public:
    int i = 0;
};


void testEvent() {
    EventListenerImpl listener;
    doorOpener.addListener(&listener);
    doorOpener.openDoor();
    TEST_ASSERT_EQUAL_INT(1, listener.i);
    doorOpener.removeListener(&listener);
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testDefault);

    RUN_TEST(testUnlock);

    RUN_TEST(testWhileUnlocked);

    RUN_TEST(testDefault);

    RUN_TEST(testUnlockDuration);

    RUN_TEST(testEvent);

    UNITY_END();
}

void loop() {
    // No need.
}
