#pragma once


#include <Arduino.h>


#include "NetworkConnection.hpp"


namespace DC {

    /**
     * @brief Manages a WiFi connection.
     * 
     * @author David Gruen
     */
    class WiFiSingleton : public NetworkConnection {

    public:

        /**
         * @brief Set the network credentials
         * 
         * @param ssid The ssid of the network.
         * @param password The password of the network.
         */
        void setCredentials(const String& ssid, const String& password);


        /**
         * @brief Get the Instance object.
         * 
         * @return The instance.
         */
        static WiFiSingleton& getInstance();


        /**
         * @brief Checks if the network is connected.
         * 
         * @return true 
         * @return false 
         */
        bool isConnected() override;


        /**
         * @brief Connects to the network.
         * 
         */
        void connect() override;


        /**
         * @brief Disconnects from the network.
         * 
         */
        void disconnect() override;


        /**
         * @brief Returns the IP address of the device in the network.
         * 
         * @return IPAddress 
         */
        IPAddress getIPAddress() override;

    private:

        // private constructor
        WiFiSingleton();

    private:

        String ssid;
        String password;

    };

}
