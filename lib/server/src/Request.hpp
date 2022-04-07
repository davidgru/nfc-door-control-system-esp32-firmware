#pragma once 


#include <Arduino.h>
#include <utility>


#include <types.hpp>


namespace DC {



    /**
     * @brief Interface for a Request
     * 
     * @author David Gruen
     */
    class Request {

    public:

        /**
         * @brief Get the Method object.
         * 
         * @return Method 
         */
        virtual Method getMethod() const = 0;


        /**
         * @brief Get the Body object.
         * 
         * @return const Body& 
         */
        virtual Body getBody() const = 0;


        /**
         * @brief Check if the request has a header.
         * 
         * @param name The key of the header.
         * @return true If the request has the header.
         * @return false Else.
         */
        virtual bool hasHeader(const String& name) const = 0;


        /**
         * @brief Get a header value by it's key.
         * 
         * @param name The key of the header.
         * @return The value of the header.
         */
        virtual String getHeader(const String& name) const = 0;

    };


};
