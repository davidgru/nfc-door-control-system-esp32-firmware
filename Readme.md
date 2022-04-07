
This project is part of the nfc-door-controll-system: https://github.com/l-nn-rt/nfc-door-controll-system

# Software requirements
* PlatformIO to upload the software to the esp32 (We recommend PlatformIO extension for vscode)

# Hardware requirements

* ESP32 microcontroller
* RC522 NFC-Reader module. A different module can be used (eg the pn532 module). Then the [NFCReader](./lib/nfc/src/NFCReader.cpp) class  has to be changed.

# Wiring

* A relay can be connected to DC_DOOR_OPEN_PIN and GND. HIGH is given to that pin to open the door. Behavior of the pin can be modified in the [Door](./lib/door/src/DoorOpener.cpp) class.
* A bell can be connected to BELL_PIN and GND. To ring the bell LOW has to be given to the pin. Behavior of the pin can be modified in the [Doorbell](./lib/door/src/Doorbell.cpp) class.
* A alarm device can be connected to ALARM_PIN and GND. The esp gives high to that pin if the Doorbell rings. Behavior can be changed in the [Alarm](./lib/notification/src/Alarm.cpp) class.
* Wiring of the NFC-Reader module according to the following diagram (in case RC522 is being used):
![Alt text](./esp32-rfid-rc522-wiring-diagram.jpg)

# Installation

* Replace [certificate.pem.crt](./certs/certificate.pem.crt) and [private.pem.key](./certs/private.pem.key) with your own ssl certificate and key. The files are only recognized if they are named exactly like the placeholders.
* Go to build_flags in [platformio.ini](./platformio.ini) and change the following macros:
    * WIFI_SSID and WIFI_PASSWORD. The esp will print it's IP address to the serial monitor if it managed to connect to WiFi.
    * DC_MDNS if you want a MDNS for the esp.
    * DC_MIDWARE_URL must be set to the base url of the midware. Including the terminating '/'!!!
    * PSKEY must be set to the same pre-shared-key as in the midware.
    * TOKEN_SEED must be set to the same token seed as in the midware.
    * Alarm and door open duration can be adjusted with DOOR_OPEN_DURATION_MILLIS and ALARM_DURATION_MILLIS respectively.
    * The remaining macros can be changed but it's not recommended. For example the capacity of the database can be increased but is bound by flash size.
* Connect the ESP32 to a USB port via Micro USB cable or USB to UART bridge.
* Use PlatformIO to upload to the esp32. To do that in vscode enter "PlatformIO: Upload" in the command palette.
