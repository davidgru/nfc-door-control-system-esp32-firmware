#include "Alarm.hpp"


#include <door.hpp>
#include <types.hpp>
#include <util.hpp>


namespace DC {


    Alarm::Alarm(Pin pin, Duration signalDuration)
        : pin(pin), alarmEndAction(signalDuration, alarmEndRoutine, this) {
        pinMode(this->pin, OUTPUT);
        digitalWrite(this->pin, LOW);
    }


    void Alarm::onEvent(const DoorbellEvent& event) {
        if (digitalRead(this->pin) == HIGH) {
            return; // Alarm on already.
        }
        digitalWrite(this->pin, HIGH);
        this->alarmEndAction.schedule();
    }


    void Alarm::onEvent(const DoorOpenEvent& event) {
        digitalWrite(this->pin, LOW);
        this->alarmEndAction.cancel();
    }


    /*static*/ void Alarm::alarmEndRoutine(Alarm* _this) {
        digitalWrite(_this->pin, LOW);
    }


}