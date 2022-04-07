#pragma once


#include "Server.hpp"
#include "DoorOpenHandler.hpp"
#include "TokenGetHandler.hpp"
#include "TokenPutHandler.hpp"
#include "TokenDeleteHandler.hpp"


namespace DC {


    class ServerFacade {

    public:

        /**
         * @brief Construct a new Server Facade object.
         * 
         * @param pskValidator A PSK validator.
         * @param doorOpener A door opener.
         * @param database A database.
         */
        ServerFacade(
            Validator<PSK>& pskValidator,
            DoorOpener& doorOpener,
            Database& database
        );


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

        Server server;
        DoorOpenHandler doorOpenHandler;
        TokenGetHandler tokenGetHandler;
        TokenPutHandler tokenPutHandler;
        TokenDeleteHandler tokenDeleteHandler;

    };


}
