#pragma once


#include <event.hpp>
#include <types.hpp>
#include <util.hpp>


namespace DC {


    struct DoorOpenEvent {
        // No information needed.
    };


    /**
     * @brief Responsible for opening a door.
     * 
     * @note For now lights a LED to signal that the door is open.
     * 
     * @author David Gruen
     */
    class DoorOpener : public EventPublisher<DoorOpenEvent> {

    public:

        /**
         * @brief Construct a new Door Opener object.
         * 
         * @param pin The door open pin.
         * @param openDuration Specifies how long the door stays open.
         */
        DoorOpener(Pin pin, Duration openDuration);


        /**
         * @brief Opens the door and published a DoorOpenEvent.
         * 
         * @note For now gives HIGH to pin for openDuration.
         */
        void openDoor();

    private:

        static void doorCloseRoutine(DoorOpener* _this);

    private:

        const Pin pin;

        DelayedAction<DoorOpener> doorCloseAction;

    };


}
