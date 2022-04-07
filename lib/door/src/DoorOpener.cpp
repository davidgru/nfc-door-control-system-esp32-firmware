#include "DoorOpener.hpp"


#include <Arduino.h>


namespace DC {


    DoorOpener::DoorOpener(Pin pin, Duration openDuration)
        : pin(pin), doorCloseAction(openDuration, doorCloseRoutine, this) {
        pinMode(this->pin, OUTPUT);
        digitalWrite(this->pin, LOW); // LED should be off initially.
    }


    void DoorOpener::openDoor() {
        if (digitalRead(this->pin) == HIGH) {
            return; // Door is already unlocked.
        }
        digitalWrite(this->pin, HIGH);
        doorCloseAction.schedule();
        this->notifyListeners(DoorOpenEvent{});
    }


    /*static*/ void DoorOpener::doorCloseRoutine(DoorOpener* _this) {
        digitalWrite(_this->pin, LOW);
    }


}
