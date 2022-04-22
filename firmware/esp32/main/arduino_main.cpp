/*******************************************************************************
  ; -----------------------------------------------------------------------
   gbaHD-ESP32 for zwenergys gbaHD

   MIT License

   Copyright (c) 2021 Alexander Kreutz

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

 *******************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>

#include <LittleFS.h>

#include "driver/sdmmc_host.h"


#include "web_handler.h"
#include "wifi_handler.h"
#include "bitstream_handler.h"
#include "mega_handler.h"
#include "ota_handler.h"
#include "log_handler.h"


static int64_t timer_5ms_timestamp = 0U;
static int64_t timer_50ms_timestamp = 0U;
static int64_t timer_100ms_timestamp = 0U;


// Arduino setup function. Runs in CPU 1
void setup() {
  // sdmmc_slot_config_t sd_config = SDMMC_SLOT_CONFIG_DEFAULT();
  // sd_config.flags = SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
  // sdmmc_host_init();
  // sdmmc_host_init_slot(1, &sd_config);
  sdmmc_host_pullup_en(1, 4); //Slot: 1 and Bit mode: 4

  Serial.begin(115200);
#ifdef GBAHD_DEBUG_OUTPUT
  Serial.setDebugOutput(true);
#endif
  // Start LittleFS
  LittleFS.begin(true);

  Preferences_Handler.init();

  // initialization 
  Bitstream_Handler.init();

  // Handle SD Card LittleFS Update and Hotboot detection
  // Bitstream_Handler.handle_sd_card();

  // Handle pushing the bitstream to Spartan
  Bitstream_Handler.handle_bit_stream();

  Mega_Handler.init();

  Wifi_Handler.init();

  Web_Handler.init();

  Log_Handler.init();

  OTA_Handler.init();
}

// Arduino loop function. Runs in CPU 1
void loop() {

  int64_t timestamp = esp_timer_get_time() / 1000;

  if ((timestamp - timer_5ms_timestamp) > 5)
  {
    Mega_Handler.update();
    timer_5ms_timestamp = timestamp;
  }
  vTaskDelay(1);

  if ((timestamp - timer_50ms_timestamp) > 50)
  {
    Web_Handler.run();
    Wifi_Handler.update();    
    
    OTA_Handler.run();
    timer_50ms_timestamp = timestamp;
  }
  vTaskDelay(1);


  if ((timestamp - timer_100ms_timestamp) > 100)
  {
    Log_Handler.run();
    
    timer_100ms_timestamp = timestamp;
  }
  vTaskDelay(1);
}

