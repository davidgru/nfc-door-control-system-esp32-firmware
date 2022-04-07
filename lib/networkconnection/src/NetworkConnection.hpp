#pragma once


#include <types.hpp>


namespace DC {


    /**
     * @brief Manages a network connection.
     * 
     * @author David Gruen
     */
    class NetworkConnection {

    public:

        /**
         * @brief Checks if the network is connected.
         * 
         * @return true 
         * @return false 
         */
        virtual bool isConnected() = 0;


        /**
         * @brief Connects to the network.
         * 
         */
        virtual void connect() = 0;


        /**
         * @brief Disconnects from the network.
         * 
         */
        virtual void disconnect() = 0;


        /**
         * @brief Returns the IP address of the device in the network.
         * 
         * @return IPAddress 
         */
        virtual IPAddress getIPAddress() = 0;

    };


}
