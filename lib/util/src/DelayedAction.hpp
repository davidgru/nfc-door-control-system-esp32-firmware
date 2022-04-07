#pragma once


#include <Arduino.h>


#include <types.hpp>


#define DELAYED_ACTION_TASK_NAME "DelayedAction task"


namespace DC {


    /**
     * @brief Executes a function in the future.
     * 
     * @tparam CallbackArgType 
     */
    template<typename CallbackArgType>
    class DelayedAction {

        using Callback = void(*)(CallbackArgType*);

    public:

        /**
         * @brief Construct a new Delayed Action object
         * 
         * @param delayMS Time between call to schedule() and callback execution.
         * @param callback Signature: void cb(CallbackArgType*)
         * @param arg Gets passed to the callback.
         */
        DelayedAction(Duration delayMS, Callback callback, CallbackArgType* arg)
            : callback(callback), arg(arg) {
            this->timer = xTimerCreate(
                DELAYED_ACTION_TASK_NAME,
                pdMS_TO_TICKS(delayMS),
                false,
                (void*)this,
                timerCallback
            );
        }


        /**
         * @brief Destroy the Delayed Action object
         * 
         */
        ~DelayedAction() {
            xTimerDelete(this->timer, portMAX_DELAY);
        }


        /**
         * @brief Executes callback after specified delay.
         * 
         */
        void schedule() {
            xTimerReset(this->timer, portMAX_DELAY);
        }


        /**
         * @brief Tries to cancel the operation.
         * 
         */
        void cancel() {
            xTimerStop(this->timer, portMAX_DELAY);
        }

    private:

        static void timerCallback(TimerHandle_t xTimer) {
            DelayedAction* _this = (DelayedAction*)pvTimerGetTimerID(xTimer);
            _this->callback(_this->arg);
        }

    private:
    
        Callback callback;
        CallbackArgType* arg;

        TimerHandle_t timer;

    };


}
