#pragma once


namespace DC {


    /**
     * @brief Listens to an event.
     * 
     * @tparam Event
     * @author David Gruen
     */
    template<class Event>
    class EventListener {

    public:

        /**
         * @brief To be called on an event.
         * 
         * @param event 
         */
        virtual void onEvent(const Event& event) = 0;

    };


}
