#pragma once


#include <door.hpp>


namespace DC {


    /**
     * @brief Signals when the bell is ringing.
     * 
     * @note For now uses a LED for signalling.
     * 
     * @author David Gruen
     */
    class Alarm : public EventListener<DoorOpenEvent>,
                public EventListener<DoorbellEvent> {

    public:

        /**
         * @brief Construct a new Alarm object
         * 
         * @param pin Pin used for signalling.
         * @param signalDuration
         */
        Alarm(Pin pin, Duration signalDuration);


        /**
         * @brief Starts the signal.
         * 
         * @param event Ignored.
         */
        void onEvent(const DoorbellEvent & event) override;


        /**
         * @brief Stops the signal if there is one.
         * 
         * @param event Ignored.
         */
        void onEvent(const DoorOpenEvent& event) override;

    private:

        static void alarmEndRoutine(Alarm* _this);

    private:
        
        const Pin pin;

        DelayedAction<Alarm> alarmEndAction;

    };


}