#include "Doorbell.hpp"

#define DOORBELL_TASK_NAME "Doorbell task"
#define DOORBELL_TASK_STACK_SIZE 4096
#define DOORBELL_TASK_PRIORITY 2

#define DOORBELL_MIN_RING_INTERVAL 5000

namespace DC {


    Doorbell::Doorbell(Pin interruptPin)
        : interruptPin(interruptPin), requestTaskTerminate(false) {
        this->binarySemaphore = xSemaphoreCreateBinary();
        xTaskCreate(
            taskRoutine,
            DOORBELL_TASK_NAME,
            DOORBELL_TASK_STACK_SIZE,
            (void*)this,
            DOORBELL_TASK_PRIORITY | portPRIVILEGE_BIT,
            NULL
        );
    }


    Doorbell::~Doorbell() {
        this->stop();
        this->requestTaskTerminate = true;
        vSemaphoreDelete(this->binarySemaphore);
    }


    void Doorbell::start() {
        pinMode(digitalPinToInterrupt(this->interruptPin), INPUT_PULLUP);
        attachInterruptArg(this->interruptPin, isr, (void*)this, FALLING);
    }


    void Doorbell::stop() {
        detachInterrupt(this->interruptPin);
    }


    /*static*/ void IRAM_ATTR Doorbell::isr(void* arg) {
        Doorbell* _this = (Doorbell*)arg;
        
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(_this->binarySemaphore, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken == pdTRUE) {
            portYIELD_FROM_ISR();
        }
    }

    
    /*static*/ void Doorbell::taskRoutine(void* arg) {
        Doorbell* _this = (Doorbell*)arg;

        unsigned long lastRing = millis();

        while(!_this->requestTaskTerminate) {
            if (xSemaphoreTake(_this->binarySemaphore, portMAX_DELAY) == pdTRUE) {
                
                unsigned long now = millis();
                if (now - lastRing > DOORBELL_MIN_RING_INTERVAL) {
                    _this->notifyListeners(DoorbellEvent{});
                    lastRing = now;
                }
            }
        }
        vTaskDelete(NULL);
    }


}
