#include <Arduino.h>

#include <database.hpp>
#include <networkconnection.hpp>
#include <nfc.hpp>
#include <notification.hpp>
#include <server.hpp>

#include "config.hpp"


DC::PSK psk = PSKEY;

DC::PSKValidator validator(psk);

DC::DatabaseGuardProxy<DC_UNDERLYING_DATABASE> database(DATABASE_FILE);

DC::Alarm _alarm(ALARM_PIN, ALARM_DURATION_MILLIS);

#ifdef DC_MIDWARE_BASE_URL
DC::MidwareClient client(DC_MIDWARE_BASE_URL, psk);
#endif

DC::DoorOpener doorOpener(DOOR_OPEN_PIN, DOOR_OPEN_DURATION_MILLIS);
DC::Doorbell doorbell(DOORBELL_PIN);

DC::NFCTokenValidator nfcTokenValidator(TOKEN_SEED, database);
DC::NFCFacade nfcFacade({NFC_SS_PIN, NFC_RST_PIN}, doorOpener, nfcTokenValidator);

DC::ServerFacade serverFacade(validator, doorOpener, database);


void setup() {
  Serial.begin(115200);
  delay(2000);

  auto& wifi = DC::WiFiSingleton::getInstance();
  wifi.setCredentials(WIFI_SSID, WIFI_PASSWORD);
  wifi.connect();
  Serial.println(wifi.getIPAddress());

  doorOpener.addListener(&_alarm);
  doorbell.addListener(&_alarm);

#ifdef DC_MIDWARE_BASE_URL
  doorOpener.addListener(&client);
  doorbell.addListener(&client);
#endif

  doorbell.start();
  serverFacade.start();
  nfcFacade.start();

}

void loop() {
}
