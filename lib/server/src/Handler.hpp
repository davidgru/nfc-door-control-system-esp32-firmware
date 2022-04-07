#pragma once


#include <types.hpp>


#include "Request.hpp"
#include "Response.hpp"


namespace DC {


    /**
     * @brief Handler inteface for HTTP-Requests
     * 
     * @author David Gruen
     */
    class Handler {

    public:
    
        /**
         * @brief Return the method to handle.
         * 
         * @return The method to handle.
         */
        virtual Method getMethod() const = 0;
        

        /**
         * @brief Return the uri to handle.
         * 
         * @return The uri to handle.
         */
        virtual const URI& getURI() const = 0;


        /**
         * @brief Handles a HTTP-Request
         * 
         * @param request A request.
         * @return The response.
         */
        virtual Response onRequest(const Request& request) = 0;

    };


}