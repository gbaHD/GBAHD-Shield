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

#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"

#define MAX_NUM_CLIENTS ( 4U )

class Log_Handler_Class : public Print
{
    public:
        void init(void);
        void run(void);

        size_t write(uint8_t c);
        size_t write(const uint8_t *buffer, size_t size);

        void debugLine(String& string);
    private:
        static void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
        AsyncWebSocket* ws;
        AsyncWebSocketClient * clients[MAX_NUM_CLIENTS];
        uint8_t no_of_clients = 0U;



};

extern Log_Handler_Class Log_Handler;