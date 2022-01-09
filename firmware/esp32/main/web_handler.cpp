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

#include "web_handler.h"
#include <Arduino.h>
#include <SPIFFS.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <esp_ota_ops.h>

#include "mega_handler.h"
#include "bitstream_handler.h"
#include "preferences_handler.h"

Web_Handler_Class Web_Handler;
WebServer Web_Handler_Class::_server(80);
File Web_Handler_Class::fsUpload;
bool Web_Handler_Class::uploadSuccess = false;
int64_t Web_Handler_Class::rebootTimer = -1;


const String BT_MAPPING_OPTION = "<option value=\"{{OPTION_VALUE}}\" {{SELECTED}}>{{OPTION_TEXT}}</option>";
const String BT_MAPPING_TR = "<tr><td>{{INPUT_BTN}}</td><td><select name=\"{{SELECT_NAME}}\" id=\"{{SELECT_NAME}}\">{{MAPPING_OPTIONS}}</select></td></tr>";

const String INPUT_HTML_MAP[BT_INP_MAX] = {
    HTML_A,
    HTML_B,
    HTML_Y,
    HTML_X,
    HTML_UP,
    HTML_DOWN,
    HTML_LEFT,
    HTML_RIGHT,
    HTML_TR_L,
    HTML_TR_R,
    HTML_SH_L,
    HTML_SH_R,
    HTML_START,
    HTML_SELECT,
    HTML_SYSTEM};

void Web_Handler_Class::_handle404()
{
  _server.send(404, "text/plain", "Not found.");
}

void Web_Handler_Class::_sendOK()
{
  _server.send(200);
}


String Web_Handler_Class::build_option(uint16_t value, uint16_t mappedValue, String text)
{
  String option = BT_MAPPING_OPTION;
  option.replace("{{OPTION_VALUE}}", String(value));
  option.replace("{{SELECTED}}", (value == mappedValue) ? "selected" : "");
  option.replace("{{OPTION_TEXT}}", text);
  return option;
}

String Web_Handler_Class::build_select_entry(uint16_t &inp_btn, uint16_t &mappedValue)
{
  String select_entry = BT_MAPPING_TR;
  String options = "";

  options += build_option(CTRL_IN_UP, mappedValue, HTML_UP);
  options += build_option(CTRL_IN_DOWN, mappedValue, HTML_DOWN);
  options += build_option(CTRL_IN_LEFT, mappedValue, HTML_LEFT);
  options += build_option(CTRL_IN_RIGHT, mappedValue, HTML_RIGHT);
  options += build_option(CTRL_IN_A, mappedValue, HTML_A);
  options += build_option(CTRL_IN_B, mappedValue, HTML_B);
  options += build_option(CTRL_IN_L, mappedValue, HTML_L);
  options += build_option(CTRL_IN_R, mappedValue, HTML_R);
  options += build_option(CTRL_IN_START, mappedValue, HTML_START);
  options += build_option(CTRL_IN_SELECT, mappedValue, HTML_SELECT);
  options += build_option(ENABLE_OSD, mappedValue, HTML_OSD);

  select_entry.replace("{{INPUT_BTN}}", INPUT_HTML_MAP[inp_btn]);
  select_entry.replace("{{SELECT_NAME}}", String(inp_btn));
  select_entry.replace("{{MAPPING_OPTIONS}}", options);

  return select_entry;
}

String Web_Handler_Class::build_update_done(bool success)
{
  String page_string = "";
  String mega_version = ""; 
  esp_app_desc_t app_desc;
  {
    File page = SPIFFS.open("/webpage/update.html", "r");
    if (page)
    {
      page_string = page.readString();
      page.close();
    }
  }
  
  page_string.replace("{{STATE}}", success ? "successful" : "failed");
  Mega_Handler.get_update_version(mega_version);
  page_string.replace("{{SHIELD_VER}}", mega_version);
  
  esp_ota_get_partition_description(esp_ota_get_boot_partition(), &app_desc);
  page_string.replace("{{ESP_VER}}", app_desc.version);


  return page_string;
}

void Web_Handler_Class::handleReboot()
{
  _server.sendHeader("Location", "/");
  _server.send(303);
  rebootTimer = (esp_timer_get_time() / 1000) + 4000;
  Serial.println("Scheduled Reboot");
}

void Web_Handler_Class::handleUploadDone()
{
  _server.send(200, "text/html", build_update_done(uploadSuccess));
}

void Web_Handler_Class::handleSPIFFSFileUpload()
{
  String path = "";

  uploadSuccess = false;

  if (_server.uri() == "/upgrade/bitstream")
  {
    path = BITSTREAM_SPIFFS_PATH;
  }
  else if (_server.uri() == "/upgrade/atmega")
  {
    path = ATMEGA_SPIFFS_PATH;
  }
  
  if (path.length() > 0)
  {
    HTTPUpload &upload = _server.upload();
    Serial.println(upload.status);

    if (upload.status == UPLOAD_FILE_START)
    {
      fsUpload = SPIFFS.open(path, "w");
      if (!fsUpload)
      {
        Serial.println("Cannot open " + path + " in SPIFFS");
      }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
      if (fsUpload)
      {
        fsUpload.write(upload.buf, upload.currentSize);
      }
    }
    else if (UPLOAD_FILE_END == upload.status)
    {
      if (fsUpload)
      {
        fsUpload.close();
        uploadSuccess = true;
        if (path == ATMEGA_SPIFFS_PATH)
        {
          Mega_Handler.trigger_external_update(_server.arg("force") == "on");
        }
      }
      else
      {
        Serial.println("FAILED: File already closed.");
      }
    }
  }
  else
  {
    Serial.println("FAILED: Path empty.");
  }
}


void Web_Handler_Class::handlePartitionUpload()
{
  int partition = -1;

  uploadSuccess = false;

  if (_server.uri() == "/upgrade/esp32")
  {
    partition = U_FLASH;
  }
  else if (_server.uri() == "/upgrade/spiffs")
  {
    partition = U_SPIFFS;
  }

  HTTPUpload &upload = _server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    // Open the file to write.
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, partition))
    { //start with max available size
      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    /* flashing firmware to ESP*/
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
    {
      Update.printError(Serial);
    }
  }
  else if (UPLOAD_FILE_END == upload.status)
  {
    if (Update.end(true))
    { //true to set the size to the current progress
      Serial.printf("Update Success: %u\n", upload.totalSize);
      uploadSuccess = true;
    }
    else
    {
      Update.printError(Serial);
    }
  }
}



void Web_Handler_Class::handleBluetooth()
{
  String page_string = "";
  Serial.println("Handle Bluetooth");

  if (_server.method() == HTTP_POST)
  {
    Bluetooth_Config config = {};
    for (uint8_t i = 0U; i < _server.args(); i++)
    {
      if (_server.argName(i) == "bluetooth_enable")
      {
        config.enabled = (_server.arg(i) == "on");
      }
      else
      {
        config.mapping[_server.argName(i).toInt()] = _server.arg(i).toInt();
      }
    }
    Preferences_Handler.saveBluetoothConfig(config);
  }

  {
    File page = SPIFFS.open("/webpage/bluetooth.html", "r");
    if (page)
    {
      page_string = page.readString();
      page.close();
    }
  }

  {
    Bluetooth_Config config;
    Preferences_Handler.getBluetoothConfig(config);
    String mapping_string = "";
    for (uint16_t i = 0U; i < BT_INP_MAX; i++)
    {
      mapping_string += build_select_entry(i, config.mapping[i]);
    }
    page_string.replace("{{MAPPINGS}}", mapping_string);
    page_string.replace("{{BT_ENABLED}}", config.enabled ? "checked" : "");
  }

  _server.send(200, "text/html", page_string);
}

void Web_Handler_Class::init(void)
{

  // Handle Bitstream upload.
  _server.on("/upgrade/bitstream",  HTTP_POST, handleUploadDone, handleSPIFFSFileUpload);
  // Handle ATMega upload.
  _server.on("/upgrade/atmega",     HTTP_POST, handleUploadDone, handleSPIFFSFileUpload);

  // Handle Bitstream upload.
  _server.on("/upgrade/esp32", HTTP_POST, handleUploadDone, handlePartitionUpload);

  // Handle SPIFFS upload.
  _server.on("/upgrade/spiffs", HTTP_POST, handleUploadDone, handlePartitionUpload);

  _server.on("/reboot", HTTP_GET, handleReboot);

  // Handle bt config
  _server.on("/bluetooth.html", HTTP_GET, handleBluetooth);
  _server.on("/bluetooth.html", HTTP_POST, handleBluetooth);

  _server.serveStatic("/", SPIFFS, "/webpage/index.html");
  _server.serveStatic("/pico.min.css", SPIFFS, "/webpage/pico.min.css");
  _server.serveStatic("/Logo.png", SPIFFS, "/webpage/Logo.png");

  // Handle everything else.
  _server.onNotFound(_handle404);

  // Set up DNS.
  if (!MDNS.begin("gbahd"))
  {
    Serial.println("Error setting up DNS");
  }

  _server.begin();
}

void Web_Handler_Class::run(void)
{
  if ((rebootTimer > 0) && (rebootTimer < (esp_timer_get_time() / 1000)))
  {
    ESP.restart();
  }

  _server.handleClient();
}