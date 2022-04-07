#include "DoorOpenHandler.hpp"


#include <types.hpp>


#include "ServerConfig.hpp"


namespace DC {


    DoorOpenHandler::DoorOpenHandler(Validator<PSK>& pskValidator, DoorOpener& doorOpener)
        : HandlerThatValidatesPSK(pskValidator), doorOpener(doorOpener) {

    }


    Method DoorOpenHandler::getMethod() const {
        return Method::POST;
    }
        
    
    const URI& DoorOpenHandler::getURI() const {
        static URI uri = SERVER_DOOR_URI;
        return uri;
    }


    Response DoorOpenHandler::onAuthorizedRequest(const Request& request) {
        this->doorOpener.openDoor();
        return Response(STATUS_OK, EMPTY_RESPONSE);
    }


}
