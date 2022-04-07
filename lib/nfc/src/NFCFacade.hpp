#pragma once

#include <types.hpp>
#include <validator.hpp>

#include "NFCReader.hpp"
#include "NFCTokenHandler.hpp"

namespace DC {


    /**
     * @brief Wraps the NFC logic.
     * 
     * @author David Gruen
     */
    class NFCFacade {

    public:

        /**
         * @brief Construct a new NFCFacade object
         * 
         * @param config Pin config for the NFC-Reader.
         * @param doorOpener To open the door.
         * @param tokenValidator Validator for NFC-Tokens.
         */
        NFCFacade(const NFCPinConfig& config, DoorOpener& doorOpener, Validator<NFCToken>& tokenValidator);


        /**
         * @brief Start the NFC-Reader.
         * 
         */
        void start();


        /**
         * @brief Stop the NFC-Reader.
         * 
         */
        void stop();

    private:

        NFCTokenHandler tokenHandler;
        NFCReader nfcReader;

    };


}
