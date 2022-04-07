#include "TokenPutHandler.hpp"


#include <ArduinoJson.h>
#include <vector>


#include <types.hpp>


#include "ServerConfig.hpp"

#define TOKENS_KEY "tokens"
#define TO_REPLACE_KEY "toReplace"
#define TO_PUT_KEY "toPut"


#define ERROR_UNABLE_TO_PARSE_JSON "Error: Unable to parse JSON body: "
#define ERROR_MISSING_TOKENS_KEY ("Error: JSON missing key: '" TOKENS_KEY "'.")
#define ERROR_TOKENS_NOT_ARRAY ("Error: '" TOKENS_KEY "' must be array.")
#define ERROR_TOKENS_ARRAY_MUST_CONTAIN_ONLY_JSON_OBJECTS ("Error: '" TOKENS_KEY "' array must contain objects only.")
#define ERROR_TO_REPLACE_NOT_STRING ("Error: '" TO_REPLACE_KEY "' field must be a String.")
#define ERROR_MISSING_TO_PUT_FIELD ("Error: Each object needs a '" TO_PUT_KEY "' field.")
#define ERROR_TO_PUT_NOT_STIRNG ("Error: '" TO_PUT_KEY "' field must be a String.")
#define ERROR_TO_PUT_TOO_BIG "Error: Max allowed token string length is "
#define ERROR_TO_PUT_NO_TOKEN "Error: String is not a valid NFC-Token representation."
#define ERROR_DATABASE_FULL "Error: Request does not fit in database."
#define DOT "."


namespace DC {


    TokenPutHandler::TokenPutHandler(Validator<PSK>& pskValidator, Database& database)
        : HandlerThatValidatesPSK(pskValidator), database(database) {

    }


    Method TokenPutHandler::getMethod() const {
        return Method::PUT;
    }
        
    
    const URI& TokenPutHandler::getURI() const {
        static String uri = SERVER_TOKEN_URI;
        return uri;
    }


    Response TokenPutHandler::onAuthorizedRequest(const Request& request) {
        DynamicJsonDocument requestDoc(SERVER_JSON_DOC_SIZE);
        DeserializationError err = deserializeJson(requestDoc, request.getBody());
        if (err) {
            String msg = ERROR_UNABLE_TO_PARSE_JSON;
            msg += err.c_str();
            msg += DOT;
            return this->makeErrorResponse(STATUS_BAD_REQUEST, msg);
        }
        if (!requestDoc.containsKey(TOKENS_KEY)) {
            return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_MISSING_TOKENS_KEY);
        }
        auto tokens = requestDoc.getMember(TOKENS_KEY);
        if (!tokens.is<JsonArray>()) {
            return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_TOKENS_NOT_ARRAY);
        }
        JsonArray tokenArr = tokens.as<JsonArray>();

        Batch toReplace;
        Batch toPut;

        for (auto entry : tokenArr) {
            if (!entry.is<JsonObject>()) {
                return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_TOKENS_ARRAY_MUST_CONTAIN_ONLY_JSON_OBJECTS);
            }
            JsonObject obj = entry.as<JsonObject>();
            if (obj.containsKey(TO_REPLACE_KEY)) {
                auto replaceEntry = obj.getMember(TO_REPLACE_KEY);
                if (!replaceEntry.is<String>()) {
                    return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_TO_REPLACE_NOT_STRING);    
                }
                toReplace.add(replaceEntry.as<String>());
            }
            if (!obj.containsKey(TO_PUT_KEY)) {
                return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_MISSING_TO_PUT_FIELD);
            }
            auto toPutEntry = obj.getMember(TO_PUT_KEY);
            if (!toPutEntry.is<String>()) {
                return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_TO_PUT_NOT_STIRNG);
            }
            String toPutString = toPutEntry.as<String>();
            if (toPutString.length() > DC_MAX_TOKEN_SIZE_BYTES * 2) {
                String msg = ERROR_TO_PUT_TOO_BIG;
                msg += DC_MAX_TOKEN_SIZE_BYTES * 2;
                msg += DOT;
                return this->makeErrorResponse(STATUS_BAD_REQUEST, msg);
            }
            if (!NFCToken::isValidTokenRepr(toPutString)) {
                return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_TO_PUT_NO_TOKEN);
            }
            toPut.add(toPutString);
        }

        this->database.remove(toReplace);
        if (!this->database.fits(toPut)) {
            this->database.insert(toReplace);
            return this->makeErrorResponse(STATUS_BAD_REQUEST, ERROR_DATABASE_FULL);
        }
        this->database.insert(toPut);
        return Response(STATUS_OK, EMPTY_RESPONSE);
    }


}
