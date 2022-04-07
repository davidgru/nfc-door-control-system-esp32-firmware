#include "ServerFacade.hpp"


namespace DC {

    
    ServerFacade::ServerFacade(
        Validator<PSK>& pskValidator, 
        DoorOpener& doorOpener,
        Database& database
    )   : doorOpenHandler(pskValidator, doorOpener),
          tokenGetHandler(pskValidator, database),
          tokenPutHandler(pskValidator, database),
          tokenDeleteHandler(pskValidator, database) {
        this->server.addHandler(&this->doorOpenHandler);
        this->server.addHandler(&this->tokenGetHandler);
        this->server.addHandler(&this->tokenPutHandler);
        this->server.addHandler(&this->tokenDeleteHandler);
    }


    void ServerFacade::start() {
        this->server.start();
    }


    void ServerFacade::stop() {
        this->server.stop();
    }

}
