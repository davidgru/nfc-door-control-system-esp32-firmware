#pragma once 


#include "Request.hpp"


#include <esp_http_server.h>


namespace DC {


    /**
     * @brief Request compatible with the server.
     * 
     * @author David Gruen
     */
    class HTTPDRequest : public Request {

    public:

        /**
         * @brief Construct a new HTTPDRequest object.
         * 
         * @param request A httpd request.
         */
        HTTPDRequest(httpd_req_t* request);

        /**
         * @brief Get the Method object.
         * 
         * @return Method 
         */
        Method getMethod() const;


        /**
         * @brief Get the Body object.
         * 
         * @return const Body& 
         */
        Body getBody() const;


        /**
         * @brief Check if the request has a header.
         * 
         * @param name The key of the header.
         * @return true If the request has the header.
         * @return false Else.
         */
        bool hasHeader(const String& name) const;


        /**
         * @brief Get a header value by it's key.
         * 
         * @param name The key of the header.
         * @return The value of the header.
         */
        String getHeader(const String& name) const;

    private:
        
        httpd_req_t* request;

    };


};
