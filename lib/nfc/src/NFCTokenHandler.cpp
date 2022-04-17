#include "NFCTokenHandler.hpp"


namespace DC {


    NFCTokenHandler::NFCTokenHandler(DoorOpener& doorOpener, Validator<NFCToken>& validator)
        : doorOpener(doorOpener), validator(validator) {

    }


    void NFCTokenHandler::processToken(const NFCToken& token) {
        if (this->validator.isValid(token)) {
            this->doorOpener.openDoor();
        } else {
            delay(DELAY_ON_INVALID_TOKEN_MS);
        }
    }


}
