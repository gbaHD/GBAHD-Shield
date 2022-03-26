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

#include "bitstream_handler.h"
#include <SD_MMC.h>
#include <LittleFS.h>
#include <Arduino.h>

#include "preferences_handler.h"
#include "log_handler.h"

// #define DEV_BOARD

#define XFPGA_RESET_TIMEOUT ( 200 )

/* pin define */
#define XFPGA_CCLK_PIN 17
#define XFPGA_DIN_PIN 27
#define XFPGA_PROGRAM_PIN 25
#define XFPGA_INTB_PIN 26
#define XFPGA_DONE_PIN 34
 
/* Chunk size for reading/writing bitstream */
#define CHUNK_SIZE 256

Bitstream_Handler_Class Bitstream_Handler;

// extern "C" {
//   //external header for the pulling up of SD pins at the initialization
//   #include "driver/sdmmc_host.h"
// }

// initialization
void Bitstream_Handler_Class::init(void) {

  uint16_t reset_counter = 0U;
#ifndef DEV_BOARD
  // initialize serial communication at 115200 bits per second: 

  //PULL UP of SD card pins <--- preventing mounting failure due to floating state
//  sdmmc_host_pullup_en(1, 4); //Slot: 1 and Bit mode: 4
  
  // ESP/SD pulled up pins list as reference:
  // ESP GPIO2        (pin 22) <---> SD CARD D0
  // ESP GPIO4        (pin 24) <---> SD CARD D1
  // ESP GPIO12/MTDI  (pin 18) <---> SD CARD D2
  // ESP GPIO13/MTCK  (pin 20) <---> SD CARD D3
  // ESP GPIO15/MTDO  (pin 21) <---> SD CARD CMD


  // XFPGA pin Initialize
  pinMode(XFPGA_INTB_PIN, INPUT);
  pinMode(XFPGA_DONE_PIN, INPUT);
  pinMode(XFPGA_PROGRAM_PIN, OUTPUT);

  // FPGA configuration start sign
  digitalWrite(XFPGA_PROGRAM_PIN, LOW);
  pinMode(XFPGA_CCLK_PIN, OUTPUT);
  digitalWrite(XFPGA_CCLK_PIN, LOW);
  digitalWrite(XFPGA_PROGRAM_PIN, HIGH);

  // wait until fpga reports reset complete
  while(digitalRead(XFPGA_INTB_PIN) == 0) 
  {
    if (reset_counter++ > XFPGA_RESET_TIMEOUT)
    {
      break;
    }
    delay(10);
  }
#endif
}


// load bitstream from file to FPGA
bool Bitstream_Handler_Class::pushBitStream(File& file) {
  char byte_buff[1024];
  uint8_t ident_idx = 0U;

  Log_Handler.print("Loading FPGA ... ");
  pinMode(XFPGA_DIN_PIN, OUTPUT);

  while(file.available()){

    int cnt = file.readBytes(byte_buff, sizeof(byte_buff));

    for(int i=0; i<cnt; i++){ 
     // shiftOut(XFPGA_DIN_PIN, XFPGA_CCLK_PIN, MSBFIRST, byte_buff[i]);
      unsigned char byte = static_cast<unsigned char>(byte_buff[i]);
      for(int j = 0;j < 8;j++) {
        REG_WRITE(GPIO_OUT_W1TC_REG, (1<<XFPGA_CCLK_PIN));
        REG_WRITE((byte&0x80)?GPIO_OUT_W1TS_REG:GPIO_OUT_W1TC_REG, (1<<XFPGA_DIN_PIN));
        byte = byte << 1;
        REG_WRITE(GPIO_OUT_W1TS_REG, (1<<XFPGA_CCLK_PIN));
      }

      if ((ident_idx < 4))
      {
        if (byte_buff[i] == BITSTREAM_VERSION_IDENT[ident_idx])
        {
          ident_idx++;
        }
        else
        {
          ident_idx = 0U;
        }
      }
      else if (ident_idx < 8)
      {
        version[ident_idx++ - 4] = byte_buff[i];
      }
    }
  }
  
  Log_Handler.println("DONE!");
  digitalWrite(XFPGA_CCLK_PIN, LOW); 
  
  file.close();

	
  // check the result
  if (0 == digitalRead(XFPGA_DONE_PIN)) 
  {
    Log_Handler.println("FPGA Configuration Failed");
    return false;
  }
  else 
  {
    Log_Handler.println("FPGA Configuration success");
    return true;
  }
}

void Bitstream_Handler_Class::handle_bit_stream(void)
{
  // #ifndef DEV_BOARD
  //   if ((SD_MMC.begin() && (SD_MMC.exists(BITSTREAM_SD_HOTBOOT_FILE))))
  //   {
  //       File file = SD_MMC.open(BITSTREAM_SD_HOTBOOT_FILE, "r");
  //       this->pushBitStream(file);
  //   }
  //   else if ((LittleFS.exists(BITSTREAM_SPIFFS_PATH)))
  //   {
  //       File file = LittleFS.open(BITSTREAM_SPIFFS_PATH, "r");
  //       this->pushBitStream(file);
  //   }
  //   else
  //   {
  //     Serial.println("No Bitstream available. Please Upload Bitstream.");
  //   }
  //   #endif
  Settings settings = {};
  String bitstream_path = "";
  Preferences_Handler.getSettings(settings);

  if (settings.bitstream == BITSTREAM_720P)
  {
    bitstream_path = BITSTREAM_720P_PATH;
  }
  else if (settings.bitstream == BITSTREAM_1080P)
  {
    bitstream_path = BITSTREAM_1080P_PATH;
  }
  else
  {
    bitstream_path = BITSTREAM_SPIFFS_PATH;
  }

  File file = LittleFS.open(bitstream_path, "r");
  if (file)
  {
    this->pushBitStream(file);
  }
  file.close();
}

void Bitstream_Handler_Class::get_current_version(String& version)
{
  version = String(this->version);
}

