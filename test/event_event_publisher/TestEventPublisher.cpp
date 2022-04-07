
#include <Arduino.h>
#include <unity.h>


#include "EventPublisher.hpp"

// This test uses an integer to determine if onEvent has been called.

struct EventImpl {
    int i;
};


class EventListenerImpl : public DC::EventListener<EventImpl> {
public:
    void onEvent(const EventImpl& event) override {
        this->i = event.i;
    }

public:
    int i;
};



EventListenerImpl listener;
DC::EventPublisher<EventImpl> publisher;


void setUp() {
    listener.i = 0;
}


void testAddListener() {
    publisher.addListener(&listener);
    publisher.notifyListeners(EventImpl{42});
    TEST_ASSERT_EQUAL_INT(42, listener.i);
}


void testRemoveListener() {
    publisher.removeListener(&listener);
    publisher.notifyListeners(EventImpl{43});
    TEST_ASSERT_EQUAL_INT(0, listener.i);
}



void setup() {
    delay(2000);
    UNITY_BEGIN();
}


int iteration = 0;
int max_iterations = 5;


void loop() {
    if (iteration < max_iterations) {
        RUN_TEST(testAddListener);
        RUN_TEST(testRemoveListener);
        iteration++;
    } else if (iteration == max_iterations) {
        UNITY_END();
    }
}
