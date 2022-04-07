#pragma once


#include <Arduino.h>


#include "types.hpp"


#define STATUS_OK 200
#define STATUS_BAD_REQUEST 400
#define STATUS_UNAUTHORIZED 401


namespace DC {


    /**
     * @brief HTTP Status Code
     * 
     */
    using StatusCode = int;



    /**
     * @brief A HTTP Response
     *
     * @author David Gruen 
     */
    class Response {

    public:

        /**
         * @brief Construct a new Response object.
         * 
         * @param statusCode 
         * @param body Payload data.
         */
        Response(StatusCode statusCode, const Body& body);
        

        /**
         * @brief Get the Status Code object.
         * 
         */
        StatusCode getStatusCode() const;


        /**
         * @brief Get the Body object.
         * 
         */
        const Body& getBody() const;

    public:

        const StatusCode statusCode;
        const String body;

    };


}