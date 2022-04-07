#include "TokenGetHandler.hpp"


#include <ArduinoJson.h>


#include <types.hpp>


#include "ServerConfig.hpp"


#define HASH_KEY "hash"


namespace DC {


    TokenGetHandler::TokenGetHandler(Validator<PSK>& pskValidator, const Database& database)
        : HandlerThatValidatesPSK(pskValidator), database(database) {

    }


    Method TokenGetHandler::getMethod() const {
        return Method::GET;
    }
        
    
    const URI& TokenGetHandler::getURI() const {
        static String uri = SERVER_TOKEN_URI;
        return uri;
    }


    Response TokenGetHandler::onAuthorizedRequest(const Request& request) {
        Hash hash = this->database.hash();

        DynamicJsonDocument doc(SERVER_JSON_DOC_SIZE);
        doc[HASH_KEY] = hash;

        String jsonString;
        serializeJsonPretty(doc, jsonString);
        
        return Response(STATUS_OK, jsonString);
    }

}
