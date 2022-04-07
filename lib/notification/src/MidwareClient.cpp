#include "MidwareClient.hpp"


#include <networkconnection.hpp>


#define MIDWARE_REGISTER_ROUTE "api/doorcontroller/login/"
#define MIDWARE_BELL_EVENT_ROUTE "api/events/"
#define MIDWARE_DOOR_OPEN_EVENT_ROUTE "api/events/"

#define MIDWARE_POST "POST"
#define MIDWARE_PUT "PUT"

#define MIDWARE_PSK_HEADER_KEY "psk"
#define MIDWARE_CONTENT_TYPE_HEADER_KEY "Content-Type"


#define BELL_KEY "type"
#define BELL_MSG "BELL_RUNG"

#define DOOR_KEY "type"
#define DOOR_MSG "DOOR_OPENED"

#define MIDWARE_CONTENT_TYPE "application/json"

#define MIDWARE_JSON_DOC_SIZE 512
#define MIDWARE_CLIENT_TASK_NAME "midware-client"
#define MIDWARE_CLIENT_STACK_SIZE 8192
#define MIDWARE_CLIENT_TASK_PRIO 3

#define MIDWARE_CLIENT_REQUEST_QUEUE_LENGTH 5
#define MIDWARE_CLIENT_MAX_WAIT_MS 5000

namespace DC {


    MidwareClient::MidwareClient(const URL& midwareURL, const PSK& psk) {
        this->state = new State{
            midwareURL,
            psk,
            xQueueCreate(MIDWARE_CLIENT_REQUEST_QUEUE_LENGTH, sizeof(RequestInfo*)),
            false
        };
        xTaskCreate(
            requestSendRoutine,
            MIDWARE_CLIENT_TASK_NAME,
            MIDWARE_CLIENT_STACK_SIZE,
            (void*)this->state,
            MIDWARE_CLIENT_TASK_PRIO | portPRIVILEGE_BIT,
            NULL
        );
    }


    MidwareClient::~MidwareClient() {
        this->state->requestTaskTerminate = true;
        xQueueSend(
            this->state->queue,
            NULL,
            MIDWARE_CLIENT_MAX_WAIT_MS / portTICK_PERIOD_MS
        );
    }


    void MidwareClient::onEvent(const DoorbellEvent& event) {
        String payload;

        DynamicJsonDocument doc(MIDWARE_JSON_DOC_SIZE);
        doc[BELL_KEY] = BELL_MSG;
        serializeJsonPretty(doc, payload);

        this->sendRequest(MIDWARE_BELL_EVENT_ROUTE, MIDWARE_POST, payload);

    }


    void MidwareClient::onEvent(const DoorOpenEvent& event) {
        String payload;

        DynamicJsonDocument doc(MIDWARE_JSON_DOC_SIZE);
        doc[DOOR_KEY] = DOOR_MSG;
        serializeJsonPretty(doc, payload);

        this->sendRequest(MIDWARE_DOOR_OPEN_EVENT_ROUTE, MIDWARE_POST, payload);
    }


    void MidwareClient::sendRequest(const String& uri, const String& method, const String& payload) {
        RequestInfo* request = new RequestInfo{
            .uri = uri, 
            .method = method, 
            .payload = payload
        };
        BaseType_t status = xQueueSend(
            this->state->queue,
            &request,
            MIDWARE_CLIENT_MAX_WAIT_MS / portTICK_PERIOD_MS
        );
        if (status == errQUEUE_FULL) {
            delete request;
        }
    }


    /*static*/ void MidwareClient::requestSendRoutine(void* arg) {
        State* state = (State*)arg;
        WiFiSingleton& wifi = WiFiSingleton::getInstance();
        
        RequestInfo* info;
        while(!state->requestTaskTerminate) {
            auto received = xQueueReceive(
                state->queue,
                &info,
                portMAX_DELAY
            );
            if (received == pdTRUE && info != NULL) {
                if (!wifi.isConnected()) {
                    xQueueReset(state->queue);
                }
        
                HTTPClient client;
                client.begin(state->midwareURL + info->uri);
                client.addHeader(MIDWARE_PSK_HEADER_KEY, state->psk);
                client.addHeader(MIDWARE_CONTENT_TYPE_HEADER_KEY, MIDWARE_CONTENT_TYPE);
        
                if (client.sendRequest(info->method.c_str(), info->payload) < 0) {
                    xQueueReset(state->queue);
                }

                client.end();
                
                delete info;
            }
        }
        vQueueDelete(state->queue);
        delete state;
        vTaskDelete(NULL);
    }

}
