#pragma once


#include <Database.hpp>


#include "HandlerThatValidatesPSK.hpp"


namespace DC {


    class TokenPutHandler : public HandlerThatValidatesPSK {

    public:

        /**
         * @brief Construct a new Token Put Handler object.
         * 
         * @param pskValidator A PSK validator.
         * @param database Database containing valid tokens.
         */
        TokenPutHandler(Validator<PSK>& pskValidator, Database& database);


        /**
         * @brief Return the method to handle.
         * 
         * @return The method to handle.
         */
        Method getMethod() const override;
        

        /**
         * @brief Return the uri to handle.
         * 
         * @return The uri to handle.
         */
        const URI& getURI() const override;


        /**
         * @brief Gets called when the handler is called with valid PSK.
         * 
         * @param request A validated request.
         * @return The response.
         */
        Response onAuthorizedRequest(const Request& request) override;

    private:

        Database& database;        

    };


}