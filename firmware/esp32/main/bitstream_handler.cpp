#include "bitstream_handler.h"
#include <SD_MMC.h>
#include <SPIFFS.h>


//#define DEV_BOARD

/* pin define */
#define XFPGA_CCLK_PIN 17
#define XFPGA_DIN_PIN 27
#define XFPGA_PROGRAM_PIN 25
#define XFPGA_INTB_PIN 26
#define XFPGA_DONE_PIN 34
 
/* Chunk size for reading/writing bitstream */
#define CHUNK_SIZE 256

Bitstream_Handler_Class Bitstream_Handler;

extern "C" {
  //external header for the pulling up of SD pins at the initialization
  #include "driver/sdmmc_host.h"
}

// initialization
void Bitstream_Handler_Class::init(void) {

#ifndef DEV_BOARD
  // initialize serial communication at 115200 bits per second: 

  //PULL UP of SD card pins <--- preventing mounting failure due to floating state
  sdmmc_host_pullup_en(1, 4); //Slot: 1 and Bit mode: 4
  
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
  while(digitalRead(XFPGA_INTB_PIN) == 0) {}
#endif
}


// load bitstream from file to FPGA
bool Bitstream_Handler_Class::pushBitStream(File& file) {
  char byte_buff[1024];  

  Serial.print("Loading FPGA ... ");
  pinMode(XFPGA_DIN_PIN, OUTPUT);

  while(file.available()){
     int cnt = file.readBytes(byte_buff, sizeof(byte_buff));
     for(int i=0; i<cnt; i++){ 
      shiftOut(XFPGA_DIN_PIN, XFPGA_CCLK_PIN, MSBFIRST, byte_buff[i]);
     }
  }
  
  Serial.println("DONE!");
  digitalWrite(XFPGA_CCLK_PIN, LOW); 
  
  file.close();

	
  // check the result
  if (0 == digitalRead(XFPGA_DONE_PIN)) 
  {
    Serial.println("FPGA Configuration Failed");
    return false;
  }
  else 
  {
    Serial.println("FPGA Configuration success");
    return true;
  }
}

void Bitstream_Handler_Class::handle_sd_card(void)
{
  #ifndef DEV_BOARD
    if (SD_MMC.begin())
    {
        Serial.println("SD Card available! Checking for update...");

        if (SD_MMC.exists(BITSTREAM_SD_UPDATE_FILE))
        {
            // if (SPIFFS.begin(true))
            // {
              Serial.println("Update available, copying to SPIFFS");

              unsigned char buffer[CHUNK_SIZE];
              int bytes_read;

              File SD_File = SD_MMC.open(BITSTREAM_SD_UPDATE_FILE, "r");
              File SPIFFS_File = SPIFFS.open(BITSTREAM_SPIFFS_PATH, "w+");

              do
              {
                  Serial.print(".");
                  bytes_read = SD_File.read(buffer, CHUNK_SIZE);
                  SPIFFS_File.write(buffer, CHUNK_SIZE);

              } while (bytes_read > 0);

              SPIFFS_File.flush();

              SD_File.close();
              SPIFFS_File.close();

              Serial.println("Done!");
              Serial.println("Renaming Update File.");

              SD_MMC.rename(BITSTREAM_SD_UPDATE_FILE, BITSTREAM_SD_UPDATE_FILE_OLD);

          //    SPIFFS.end();
          // }
          // else
          // {
          //   Serial.println("Error mounting SPIFFS. Please reflash using Serial flashing.");
          // }
        }
        SD_MMC.end();
    }
  #endif
}

void Bitstream_Handler_Class::handle_bit_stream(void)
{
  #ifndef DEV_BOARD
    if ((SD_MMC.begin() && (SD_MMC.exists(BITSTREAM_SD_HOTBOOT_FILE))))
    {
        File file = SD_MMC.open(BITSTREAM_SD_HOTBOOT_FILE, "r");
        this->pushBitStream(file);
    }
    else if ((SPIFFS.exists(BITSTREAM_SPIFFS_PATH)))
    {
        File file = SPIFFS.open(BITSTREAM_SPIFFS_PATH, "r");
        this->pushBitStream(file);
    }
    else
    {
      Serial.println("No Bitstream available. Please Upload Bitstream.");
    }
    #endif
}

