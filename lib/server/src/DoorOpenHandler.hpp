#pragma once


#include <door.hpp>


#include "HandlerThatValidatesPSK.hpp"


namespace DC {


    class DoorOpenHandler : public HandlerThatValidatesPSK {

    public:

        /**
         * @brief Construct a new Door Open Handler object.
         * 
         * @param pskValidator A PSK validator.
         * @param doorOpener A Door Opener.
         */
        DoorOpenHandler(Validator<PSK>& pskValidator, DoorOpener& doorOpener);


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

        DoorOpener& doorOpener;        

    };


}