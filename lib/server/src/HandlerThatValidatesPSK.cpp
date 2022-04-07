#include "HandlerThatValidatesPSK.hpp"


#include <ArduinoJson.h>


#include "ServerConfig.hpp"


#define ERROR_MISSING_PSK "Error: Missing PSK Header."
#define ERROR_INVALID_PSK "Error: Invalid PSK."

#define MSG_KEY "msg"


namespace DC {


        HandlerThatValidatesPSK::HandlerThatValidatesPSK(Validator<PSK>& pskValidator)
            : pskValidator(pskValidator) {

        }


        Response HandlerThatValidatesPSK::onRequest(const Request& request) {
            DynamicJsonDocument responseJSON(SERVER_JSON_DOC_SIZE);

            if (!request.hasHeader(SERVER_PSK_HEADER_KEY)) {
                return this->makeErrorResponse(STATUS_UNAUTHORIZED, ERROR_MISSING_PSK);
            }
            String suppliedPSK = request.getHeader(SERVER_PSK_HEADER_KEY);
            if (!this->pskValidator.isValid(suppliedPSK)) {
                return this->makeErrorResponse(STATUS_UNAUTHORIZED, ERROR_INVALID_PSK);
            }
            return this->onAuthorizedRequest(request);
        }


        Response HandlerThatValidatesPSK::makeErrorResponse(StatusCode code, const String& msg) const {
            DynamicJsonDocument doc(SERVER_JSON_DOC_SIZE);
            doc[MSG_KEY] = msg;
            String jsonStr;
            serializeJsonPretty(doc, jsonStr);
            return Response(code, jsonStr);
        }


}
