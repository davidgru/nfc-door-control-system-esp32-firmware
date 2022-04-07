
#include <Arduino.h>
#include <unity.h>


#include <networkconnection.hpp>

#include <WiFi.h>


void setUp() {

}


void tearDown() {

}


void testConnect() {
    auto& instance = DC::WiFiSingleton::getInstance();
    instance.setCredentials(WIFI_SSID, WIFI_PASSWORD);
    instance.connect();
    TEST_ASSERT_EQUAL(instance.isConnected(), WiFi.isConnected());
}


void testIP() {
    auto& instance = DC::WiFiSingleton::getInstance();
    auto ip = instance.getIPAddress();
    TEST_ASSERT_EQUAL_STRING(ip.c_str(), WiFi.localIP().toString().c_str());
}


void testDisconnect() {
    auto& instance = DC::WiFiSingleton::getInstance();
    instance.disconnect();
    delay(1000); // Takes a while
    TEST_ASSERT_FALSE(instance.isConnected());
    TEST_ASSERT_FALSE(WiFi.isConnected());
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(testConnect);
    RUN_TEST(testIP);
    RUN_TEST(testDisconnect);


    UNITY_END();
}


void loop() {
    // No need.
}
