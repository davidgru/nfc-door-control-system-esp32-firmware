#include "TokenDeleteHandler.hpp"


#include <ArduinoJson.h>
#include <vector>


#include <types.hpp>


#include "ServerConfig.hpp"

#define DELETE_ALL_KEY "deleteAll"
#define TOKENS_KEY "tokens"

#define DOT "."

#define ERROR_UNABLE_TO_PARSE_JSON_BODY "Error: Unable to parse JSON body: "
#define ERROR_DELETE_ALL_NO_BOOLEAN ("Error '" DELETE_ALL_KEY "' must be of boolean type.")
#define ERROR_TOKENS_NOT_ARRAY ("Error: '" TOKENS_KEY "' must be of array type.")
#define ERROR_TOKEN_ENTRIES_NOT_STRING "Error: tokens must be of String type."

namespace DC {


    TokenDeleteHandler::TokenDeleteHandler(Validator<PSK>& pskValidator, Database& database)
        : HandlerThatValidatesPSK(pskValidator), database(database) {

    }


    Method TokenDeleteHandler::getMethod() const {
        return Method::DELETE;
    }
        
    
    const URI& TokenDeleteHandler::getURI() const {
        static String uri = SERVER_TOKEN_URI;
        return uri;
    }


    Response TokenDeleteHandler::onAuthorizedRequest(const Request& request) {
        DynamicJsonDocument requestDoc(SERVER_JSON_DOC_SIZE);
        DeserializationError err = deserializeJson(requestDoc, request.getBody());
        if (err) {
            String msg = ERROR_UNABLE_TO_PARSE_JSON_BODY;
            msg += err.c_str();
            msg += DOT;
            return this->makeErrorResponse(STATUS_BAD_REQUEST, msg);
        }
        
        if (requestDoc.containsKey(DELETE_ALL_KEY)) {
            auto deleteAllEntry = requestDoc.getMember(DELETE_ALL_KEY);
            if (!deleteAllEntry.is<bool>()) {
                return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_DELETE_ALL_NO_BOOLEAN);
            }
            if (deleteAllEntry.as<bool>()) {
                this->database.clear();
                return Response(STATUS_OK, EMPTY_RESPONSE);
            }
        }

        Batch toDelete;
        
        if (requestDoc.containsKey(TOKENS_KEY)) {
            auto tokensEntry = requestDoc.getMember(TOKENS_KEY);
            if (!tokensEntry.is<JsonArray>()) {
                return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_TOKENS_NOT_ARRAY);
            }
            JsonArray tokensArray = tokensEntry.as<JsonArray>();
            for (auto entry : tokensArray) {
                if (!entry.is<String>()) {
                    return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_TOKEN_ENTRIES_NOT_STRING);
                }
                toDelete.add(entry.as<String>());
            }
        }

        this->database.remove(toDelete);

        return Response(STATUS_OK, EMPTY_RESPONSE);
    }


}
