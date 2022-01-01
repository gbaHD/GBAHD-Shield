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

// Include Guards
#ifndef BITSTREAM_HANDLER_H
#define BITSTREAM_HANDLER_H 
 
#include <Arduino.h>
#include <FS.h>

/* */
#define BITSTREAM_SD_UPDATE_FILE        "/update.bit"
#define BITSTREAM_SD_UPDATE_FILE_OLD    "/update.done"
#define BITSTREAM_SD_HOTBOOT_FILE       "/hotboot.bit"
#define BITSTREAM_SPIFFS_PATH           "/bitstream.bit"

class Bitstream_Handler_Class {
  public:
    void init(void);	
    void handle_sd_card(void);
    void handle_bit_stream(void);

  private: 
    bool pushBitStream(File& file);
};

extern Bitstream_Handler_Class Bitstream_Handler;

#endif
