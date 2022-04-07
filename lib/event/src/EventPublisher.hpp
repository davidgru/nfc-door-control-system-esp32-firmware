#pragma once


#include <list>


#include "EventListener.hpp"


namespace DC {


    /**
     * @brief Distributes events to its listeners.
     * 
     * @tparam Event
     * @author David Gruen
     */
    template<class Event>
    class EventPublisher {

    public:

        /**
         * @brief Add an event listener
         * 
         * @param listener An event listener (must not be null).
         */
        void addListener(EventListener<Event>* listener) {
            this->listeners.push_back(listener);
        }


        /**
         * @brief Remove an event listener.
         * 
         * @param listener An event listener.
         */
        void removeListener(EventListener<Event>* listener) {
            this->listeners.remove(listener);
        }


        /**
         * @brief Notify all subscribed event listeners.
         * 
         * @param event An event.
         */
        void notifyListeners(const Event& event) const {
            for (auto* listener : this->listeners) {
                listener->onEvent(event);
            }
        }


    private:

        std::list<EventListener<Event>*> listeners;

    };


}
