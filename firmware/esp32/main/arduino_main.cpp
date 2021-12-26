/****************************************************************************
http://retro.moe/unijoysticle2

Copyright 2021 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>

#include <SPIFFS.h>

#include "web_handler.h"
#include "wifi_handler.h"
#include "bitstream_handler.h"
#include "mega_handler.h"


static int64_t timer_5ms_timestamp = 0U;
static int64_t timer_50ms_timestamp = 0U;


// Arduino setup function. Runs in CPU 1
void setup() {
  Serial.begin(115200);

  // Start SPIFFS
  SPIFFS.begin(true);

  // initialization 
  Bitstream_Handler.init();

  // Handle SD Card SPIFFS Update and Hotboot detection
  Bitstream_Handler.handle_sd_card();

  // Handle pushing the bitstream to Spartan
  Bitstream_Handler.handle_bit_stream();

  Mega_Handler.init();

  Wifi_Handler.init();

  Web_Handler.init();

}

// Arduino loop function. Runs in CPU 1
void loop() {

  int64_t timestamp = esp_timer_get_time() / 1000;

  if ((timestamp - timer_5ms_timestamp) > 5)
  {
    Mega_Handler.update();
    timer_5ms_timestamp = timestamp;
  }

  if ((timestamp - timer_50ms_timestamp) > 50)
  {
    Web_Handler.run();
    Wifi_Handler.update();
    timer_50ms_timestamp = timestamp;
  }
}

