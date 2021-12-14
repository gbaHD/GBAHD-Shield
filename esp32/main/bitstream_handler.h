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


class bitstream_handler {
  public:
    void init(void);	
    void handle_sd_card(void);
    void handle_bit_stream(void);

  private: 
    bool pushBitStream(File& file);
};
#endif
