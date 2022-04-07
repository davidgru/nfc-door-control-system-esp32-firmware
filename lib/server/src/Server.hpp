#pragma once


#include "Handler.hpp"


#include <esp_https_server.h>
#include <list>


namespace DC {


    /**
     * @brief A HTTP-Server to receive requests from the
     * midware.
     * 
     * @author David Gruen
     */
    class Server {

    public:

        /**
         * @brief Construct a new Server object.
         * 
         */
        Server();


        /**
         * @brief Add an handler.
         * 
         * @param handler An handler (not null).
         */
        void addHandler(Handler* handler);


        /**
         * @brief Remove an handler.
         * 
         * @param handler An handler.
         */
        void removeHandler(Handler* handler);


        /**
         * @brief Start the server.
         * 
         */
        void start();
        

        /**
         * @brief Stop the server.
         * 
         */
        void stop();
    

    private:

        static esp_err_t universalHandler(httpd_req_t* req);

        static httpd_method_t toHTTPDMethod(Method method);

    private:

        httpd_handle_t serverHandle;
        httpd_ssl_config_t config;
        std::list<httpd_uri_t> uriHandlers;

    };


}
