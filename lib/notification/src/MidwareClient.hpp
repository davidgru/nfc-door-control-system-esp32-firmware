#pragma once


#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>


#include <door.hpp>
#include <types.hpp>


namespace DC {


    /**
     * @brief Communicates with the midware.
     * 
     * @author David Gruen
     */
    class MidwareClient : public EventListener<DoorbellEvent>,
                          public EventListener<DoorOpenEvent> {

    public:

        /**
         * @brief Construct a new Midware Client object.
         * 
         * @param midwareURL The url of the midware.
         * @param psk Pre-shared key.
         */
        MidwareClient(const URL& midwareURL, const PSK& psk);


        /**
         * @brief Destroy the Midware Client object.
         * 
         */
        ~MidwareClient();

        
        /**
         * @brief Tells the midware that the doorbell rings.
         * 
         * @param event Ignored.
         */
        void onEvent(const DoorbellEvent& event) override;


        /**
         * @brief Tells the midware that the door was unlocked.
         * 
         * @param event Ignored.
         */
        void onEvent(const DoorOpenEvent& event) override;

    private:

        void sendRequest(const String& uri, const String& method, const String& payload);

        static void requestSendRoutine(void* arg);

    private:

        struct State {
            URL midwareURL;
            PSK psk;
            QueueHandle_t queue;
            bool requestTaskTerminate;
        };

        struct RequestInfo {
            String uri;
            String method;
            String payload;
        };

        State* state;

        QueueHandle_t queue;
        TaskHandle_t task;

        bool requestTaskTerminate;
    };

}
