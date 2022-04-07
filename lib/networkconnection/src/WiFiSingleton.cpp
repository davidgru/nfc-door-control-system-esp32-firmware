#include "WiFiSingleton.hpp"

#include <WiFi.h>
#ifdef DC_MDNS
#include <ESPmDNS.h>
#endif

#define WIFI_SINGLETON_DEFAULT_SSID ""
#define WIFI_SINGLETON_DEFAULT_PASSWORD ""


namespace DC {


    WiFiSingleton::WiFiSingleton()
        : ssid(WIFI_SINGLETON_DEFAULT_SSID), password(WIFI_SINGLETON_DEFAULT_PASSWORD) {

    }


    void WiFiSingleton::setCredentials(const String& ssid, const String& password) {
        this->ssid = ssid;
        this->password = password;
    }


    /*static*/ WiFiSingleton& WiFiSingleton::getInstance() {
        static WiFiSingleton instance;
        return instance;
    }


    bool WiFiSingleton::isConnected() {
        return WiFi.isConnected();
    }



    void WiFiSingleton::connect() {
        WiFi.mode(WIFI_STA);
        WiFi.begin(this->ssid.c_str(), this->password.c_str());
        WiFi.waitForConnectResult();
#ifdef DC_MDNS
        MDNS.begin(DC_MDNS);
#endif
    }


    void WiFiSingleton::disconnect() {
        WiFi.disconnect();
    }


    IPAddress WiFiSingleton::getIPAddress() {
        return WiFi.localIP().toString();
    }


}