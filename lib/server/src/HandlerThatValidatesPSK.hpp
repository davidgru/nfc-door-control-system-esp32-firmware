#pragma once


#include <validator.hpp>
#include <types.hpp>


#include "Handler.hpp"


namespace DC {


    class HandlerThatValidatesPSK : public Handler {

    public:


        /**
         * @brief Construct a new Handler That Validates PSK object
         * 
         * @param pskValidator Validates PSK.
         */
        HandlerThatValidatesPSK(Validator<PSK>& pskValidator);


        /**
         * @brief If the request has a valid PSK call onAuthorizedRequest.
         * Also add the PSK to the response.
         * 
         * @param request A request.
         * @return The response.
         */
        Response onRequest(const Request& request) override;

    protected:

        /**
         * @brief Gets called when the handler is called with valid PSK.
         * 
         * @param request A validated request.
         * @return The response.
         */
        virtual Response onAuthorizedRequest(const Request& request) = 0;


        /**
         * @brief Helper method to create error responses.
         * 
         * @param code Response code.
         * @param msg Response message.
         * @return A error response.
         */
        Response makeErrorResponse(StatusCode code, const String& msg) const;


    private:

        Validator<PSK>& pskValidator;

    };


}
