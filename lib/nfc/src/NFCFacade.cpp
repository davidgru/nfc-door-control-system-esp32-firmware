#include "NFCFacade.hpp"


namespace DC {


    NFCFacade::NFCFacade(const NFCPinConfig& config, DoorOpener& doorOpener, Validator<NFCToken>& tokenValidator)
        : tokenHandler(doorOpener, tokenValidator), nfcReader(config, this->tokenHandler) {

    }


    void NFCFacade::start() {
        this->nfcReader.start();
    }


    void NFCFacade::stop() {
        this->nfcReader.stop();
    }


}
