#pragma once


#include <Arduino.h>


#include <event.hpp>
#include <types.hpp>


namespace DC {


    struct DoorbellEvent {
        // No information needed.
    };


    /**
     * @brief Listens on an interrupt pin and emits a event on trigger.
     * 
     */
    class Doorbell : public EventPublisher<DoorbellEvent> {

    public:

        /**
         * @brief Construct a new Doorbell object
         * 
         * @param interruptPin Pin to listen on.
         */
        Doorbell(Pin interruptPin);


        /**
         * @brief Destroy the Door Bell object.
         * 
         */
        ~Doorbell();


        /**
         * @brief Starts listening for interrupts.
         * 
         */
        void start();


        /**
         * @brief Stops listening for interrupts.
         * 
         */
        void stop();

    private:

        static void IRAM_ATTR isr(void* arg);


        static void taskRoutine(void* arg);

    private:

        const Pin interruptPin;

        SemaphoreHandle_t binarySemaphore;

        bool requestTaskTerminate;

    };


}
