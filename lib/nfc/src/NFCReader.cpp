#include "NFCReader.hpp"


#define NFC_READER_CHECK_INTERVAL 10

#define NFC_READER_TASK_NAME "NFC-Reader task"
#define NFC_READER_TASK_PRIORITY 5
#define NFC_READER_TASK_STACK_SIZE 4096

#define RANDOM_UID_BYTE_0 0x80


namespace DC {

    
    NFCReader::NFCReader(const NFCPinConfig& config, NFCTokenHandler& handler)
        : handler(handler), rfid(config.ssPin, config.rstPin), taskHandle(NULL) {
        SPI.begin();
        this->rfid.PCD_Init();
    }


    void NFCReader::start() {
        if (taskHandle) {
            return;
        }
        xTaskCreate(
            taskRoutine,
            NFC_READER_TASK_NAME,
            NFC_READER_TASK_STACK_SIZE,
            (void*)this,
            NFC_READER_TASK_PRIORITY | portPRIVILEGE_BIT,
            &taskHandle
        );
        if (this->taskHandle) {
            this->requestTaskStop = false;
        }
    }


    void NFCReader::stop() {
        this->requestTaskStop = true;
    }


    /*static*/ void NFCReader::taskRoutine(void* arg) {
        NFCReader* _this = (NFCReader*)arg;
        while(!_this->requestTaskStop) {
            delay(NFC_READER_CHECK_INTERVAL);

            if (!_this->rfid.PICC_IsNewCardPresent()) {
                continue;
            }

            if (!_this->rfid.PICC_ReadCardSerial()) {
                continue;
            }

            if (_this->rfid.uid.uidByte[0] == RANDOM_UID_BYTE_0) {
                continue;
            }

            NFCToken token(&_this->rfid.uid.uidByte[0], _this->rfid.uid.size);

            Serial.print("Read token: ");
            Serial.println(token.toHexRepr());

            _this->handler.processToken(token);
        }
        _this->taskHandle = NULL;
        vTaskDelete(NULL);
    }

}
