# gbaHD ESP32 Software for SAE board

This folder contains the ESP32 Software for usage with zwenergys gbaHD project.


## Features:

**Bluetooth Controller Support:**

The ESP32 Micro Controller contains an onboard wireless and Bluetooth Modem.
This Bluetooth Modem can be used to connect with a Wireless Bluetooth Controller.
This project makes use of Ricardo Quesadas *Bluepad32* library (see [2]) for the whole Bluetooth Controller communication setup.
Furthermore it uses TWI to communicate all key presses to the ATMega on the gbaHD Shield, which then publishs the key presses to the GBA.
A simplified mapping is provided in the *Settings* tab on gbaHDs Web Interface.

**OTA Update:**

To simplify all further updates by zwenergy or for the esp32, OTA functionality was added.
When opening the Web Interface on http://gbahd.local it automatically checks, if a new bitstream update or a new esp32 software is available. You can directly update using the *OTA Update* Button on the Web Interface.
To connect to a WiFi network, create a file called "wifi.cfg" with your wifi credentials on an sd card. Insert it and connection to this network will be established:
```
ssid=MyWifiSSID
password=MyWifiPasswordIsSecure
```

**SD Card Update:**

If you want to keep your device offline but still want to experience the latest and greatest features, you can simply add all Update files to SD Card. On the first boot afterwards, gbaHD will then copy these updates to its internal memory.

File            |   Update
----------------|------------
esp32.bin       | ESP32
spiffs.bin      | ESP32 data partition
720p.bit        | 720P Bitstream
1080p.bit       | 1080P Bitstream

**Bitstream Selection:**

As per default, gbaHD will be started with the 720P Bitstream. In the *Settings* tab of the Web Interface, you can change it to 1080P.


## Thanks to...

- Zwenergy for gbaHD [1]
- ManCloud for coordination and gbaHD shield
- Ricardo Quesada for Bluepad32 [2]
- Arduino and Espressif for arduino-esp32
- PicoCSS team for PicoCSS [3]
- jotheripper for his gbaHD Case
- derKevin for test feedback
- bbsan for ESP32 SW


[1] gbaHD:
https://github.com/zwenergy/gbaHD

[2] Bluepad32:
https://gitlab.com/ricardoquesada/bluepad32

[3] PicoCSS:
https://picocss.com


