/*
 * 01LoadDefaultBitstream
 *  
 * loading the default Bitstream
 *
 * The MIT License (MIT)
 * Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the library:
#include <spartan-edge-esp32-boot.h>
#include <Wire.h>
// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;

// the bitstream name which we loading
#define LOADING_DEFAULT_FIEE "/overlay/default.bit"
#define SDA_PIN 32
#define SCL_PIN 33
#define SLAVE_ADDR 0x29

#define ENABLE_TEST 1

// the setup routine runs once when you press reset:
void setup() {
  // initialization 
  esp32Cla.begin();
  

  // XFPGA pin Initialize
  esp32Cla.xfpgaGPIOInit();

  // loading the bitstream
  esp32Cla.xlibsSstream(LOADING_DEFAULT_FIEE);
  
  Serial.println("run");
#if ENABLE_TEST == 1
  Wire.begin(SDA_PIN, SCL_PIN);
  
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();
#endif
   
}

// the loop routine runs over and over again forever:
void loop() {
#if ENABLE_TEST == 1
  Serial.println();
  Serial.print("Version: ");

   Wire.beginTransmission(SLAVE_ADDR);
   Wire.write(0x01);
    Wire.endTransmission();
   Wire.requestFrom(SLAVE_ADDR, 16);

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Wire.endTransmission();
  
  Serial.println();
  Serial.print("Chip Info: ");

   Wire.beginTransmission(SLAVE_ADDR);
   Wire.write(0x02);
   Wire.write(0x00);
   Wire.write(0x00);
   Wire.write(0x00);
    Wire.endTransmission();
   Wire.requestFrom(SLAVE_ADDR, 8);

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c, HEX);         // print the character
  }
  Wire.endTransmission();
#endif

  delay(3000);
}