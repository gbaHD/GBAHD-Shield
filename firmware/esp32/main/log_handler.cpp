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

#include "log_handler.h"
#include "web_handler.h"
#include "WiFi.h"

String string_buffer = "";


void Log_Handler_Class::onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{

  if (type == WS_EVT_CONNECT)
  {
      if (Log_Handler.no_of_clients < MAX_NUM_CLIENTS)
      {
          Log_Handler.clients[Log_Handler.no_of_clients++] = client;
      }
    Log_Handler.println("Websocket client connection received");

  } 
  else if (type == WS_EVT_DISCONNECT)
  {
        uint8_t client_idx = 0U;
        for (; client_idx < Log_Handler.no_of_clients; client_idx++)
        {
            if (client == Log_Handler.clients[client_idx])
            {
                break;
            }
        }
        for (;client_idx < MAX_NUM_CLIENTS - 1; client_idx++)
        {
            Log_Handler.clients[client_idx] = Log_Handler.clients[client_idx + 1];
        }
        Log_Handler.clients[client_idx] = nullptr;
        if (Log_Handler.no_of_clients > 0)
        {
            Log_Handler.no_of_clients--;
        }
        Log_Handler.println("Websocket client connection finished");

  }
}

void Log_Handler_Class::init(void)
{   
    ws = new AsyncWebSocket("/serial");
    ws->onEvent(onWsEvent);    
    Web_Handler.addWebSocket(ws);
    for (uint8_t idx = 0U; idx < MAX_NUM_CLIENTS; idx++)
    {
        clients[idx] = nullptr;
    }
    no_of_clients = 0;
}


size_t Log_Handler_Class::write(uint8_t c)
{
    Serial.write(c);
    String s = "";
    if (c == 0x0A)
    {
        s += "<br>";
    }
    else if (c != 0x0D)
    {
        s += static_cast<char>(c);
    }
    
    string_buffer += s;

    return 1;
}

size_t Log_Handler_Class::write(const uint8_t *buffer, size_t size)
{
    String s = String(reinterpret_cast<const char*>(buffer));

    s.replace("\r\n", "<br>");

    string_buffer += s;    

    return Serial.write(buffer, size);
}

void Log_Handler_Class::debugLine(String& string)
{
#ifdef GBAHD_DEBUG_OUTPUT
    this->println(string);
#endif
}


void Log_Handler_Class::run(void)
{
    if (string_buffer.length() > 0 && ws && WiFi.isConnected())
    {
        for (uint8_t idx = 0U; idx < MAX_NUM_CLIENTS; idx++)
        {
            if (clients[idx])
            {
                clients[idx]->text(string_buffer);
            }
        }
        string_buffer.clear();
    }

    ws->cleanupClients();
}


Log_Handler_Class Log_Handler;