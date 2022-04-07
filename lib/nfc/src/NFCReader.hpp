#pragma once


#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>


#include <types.hpp>


#include "NFCTokenHandler.hpp"


namespace DC {


    /**
     * @brief Pins needed by the NFC-Reader.
     * 
     */
    struct NFCPinConfig {
        Pin ssPin;
        Pin rstPin;
    };


    /**
     * @brief Scans for NFC-Chips.
     * 
     * @author David Gruen
     */
    class NFCReader {

    public:

        /**
         * @brief Construct a new NFC-Reader object.
         * 
         * @param config Pins needed by the NFC-Reader.
         * @param handler Handler for the UID's of the detected chips.
         */
        NFCReader(const NFCPinConfig& config, NFCTokenHandler& handler);


        /**
         * @brief Start scanning for chips.
         * 
         */
        void start();


        /**
         * @brief Stop scanning for chips.
         * 
         */
        void stop();

    private:

        static void taskRoutine(void* arg);

    private:

        NFCTokenHandler& handler;

        MFRC522 rfid;
        TaskHandle_t taskHandle;

        bool requestTaskStop;

    };


}
