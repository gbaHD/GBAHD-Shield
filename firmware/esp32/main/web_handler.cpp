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
#include <LittleFS.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <esp_ota_ops.h>
#include <WiFi.h>

#include "ArduinoBluepad32.h"
#include "mega_handler.h"
#include "bitstream_handler.h"
#include "preferences_handler.h"
#include "log_handler.h"
#include "ota_handler.h"

Web_Handler_Class Web_Handler;
AsyncWebServer Web_Handler_Class::_Aserver(80);
File Web_Handler_Class::fsUpload;
bool Web_Handler_Class::uploadSuccess = false;
int64_t Web_Handler_Class::rebootTimer = -1;
int64_t Web_Handler_Class::lastAccess = -1;
bool Web_Handler_Class::serverRunning = false;

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



void Web_Handler_Class::_handle404(AsyncWebServerRequest *request)
{
  lastAccess = esp_timer_get_time();
  request->send(404, "text/plain", "Not found.");
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
  String bitstream_version = "";
  esp_app_desc_t app_desc;
  {
    File page = LittleFS.open("/webpage/update.html", "r");
    if (page)
    {
      page_string = page.readString();
      page.close();
    }
  }

  page_string.replace("{{STATE}}", success ? "successful" : "failed");
  Mega_Handler.get_update_version(mega_version);
  page_string.replace("{{SHIELD_VER}}", mega_version);

  Bitstream_Handler.get_current_version(bitstream_version);
  page_string.replace("{{BITSTREAM_VER}}", bitstream_version);

  esp_ota_get_partition_description(esp_ota_get_boot_partition(), &app_desc);
  page_string.replace("{{ESP_VER}}", app_desc.version);


  return page_string;
}

void Web_Handler_Class::handleReboot(AsyncWebServerRequest *request)
{
  request->redirect("/");
  rebootTimer = (esp_timer_get_time() / 1000) + 4000;
  Log_Handler.println("Scheduled Reboot");
}

void Web_Handler_Class::handleUploadDone(AsyncWebServerRequest *request)
{
  lastAccess = esp_timer_get_time();
  request->send(200, "text/html", build_update_done(uploadSuccess));
}

void Web_Handler_Class::handleLittleFSFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
  String path = "";

  uploadSuccess = false;

  if (request->url() == "/upgrade/bitstream")
  {
    path = BITSTREAM_SPIFFS_PATH;
  }
  else if (request->url() == "/upgrade/atmega")
  {
    path = ATMEGA_SPIFFS_PATH;
  }

  if (path.length() > 0)
  {
    if (index == 0)
    {
      fsUpload = LittleFS.open(path, "w");
      if (!fsUpload)
      {
        Log_Handler.println("Cannot open " + path + " in LittleFS");
      }
    }

    if (fsUpload)
    {
      fsUpload.write(data, len);
    }

    if (final)
    {
      fsUpload.close();
      uploadSuccess = true;
      if (path == ATMEGA_SPIFFS_PATH)
      {
        Mega_Handler.trigger_external_update(request->arg("force") == "on");
      }
    }
  }

  lastAccess = esp_timer_get_time();

}

void Web_Handler_Class::handlePartitionUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
  int partition = -1;

  uploadSuccess = false;

  if (request->url() == "/upgrade/esp32")
  {
    partition = U_FLASH;
  }
  else if (request->url() == "/upgrade/spiffs")
  {
    partition = U_SPIFFS;
  }

  if (index == 0)
  {
    // Open the file to write.
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, partition))
    { //start with max available size
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len)
  {
    Update.printError(Serial);
  }

  if (final)
  {
    if (Update.end(true))
    { //true to set the size to the current progress
      Log_Handler.println("Update Success");
      uploadSuccess = true;
    }
    else
    {
      Update.printError(Serial);
    }
  }

  lastAccess = esp_timer_get_time();

}



void Web_Handler_Class::handleSettings(AsyncWebServerRequest *request)
{
  String page_string = "";

  if (request->method() == HTTP_POST)
  {
    Bluetooth_Config config = {};
    Settings settings = {};
    for (uint8_t i = 0U; i < request->args(); i++)
    {
      if (request->argName(i) == "bluetooth_enable")
      {
        config.enabled = (request->arg(i) == "on");
      }
      else if (request->argName(i) == "bitstream")
      {
        settings.bitstream = request->arg(i).toInt();
      }
      else
      {
        config.mapping[request->argName(i).toInt()] = request->arg(i).toInt();
      }
    }
    Preferences_Handler.saveBluetoothConfig(config);
    Preferences_Handler.saveSettings(settings);

    handleReboot(request);
  }
  else
  {
    {
      File page = LittleFS.open("/webpage/settings.html", "r");
      if (page)
      {
        page_string = page.readString();
        page.close();
      }
    }

    {
      Bluetooth_Config config;
      Settings settings;
      Preferences_Handler.getBluetoothConfig(config);
      Preferences_Handler.getSettings(settings);
      String mapping_string = "";
      for (uint16_t i = 0U; i < BT_INP_MAX; i++)
      {
        mapping_string += build_select_entry(i, config.mapping[i]);
      }
      page_string.replace("{{MAPPINGS}}", mapping_string);
      page_string.replace("{{BT_ENABLED}}", config.enabled ? "checked" : "");
      page_string.replace("{{720_SELECTED}}", settings.bitstream == BITSTREAM_720P ? "selected" : "");
      page_string.replace("{{1080_SELECTED}}", settings.bitstream == BITSTREAM_1080P ? "selected" : "");
    }

    request->send(200, "text/html", page_string);
  }

  lastAccess = esp_timer_get_time();

}

void Web_Handler_Class::handleIndex(AsyncWebServerRequest *request)
{
  String page_string = "";
  {
    File page = LittleFS.open("/webpage/index.html", "r");
    if (page)
    {
      page_string = page.readString();
      page.close();
    }
  }
  {
    String gamepad_message = "";
    Mega_Handler.get_controller_name(gamepad_message);
    
    page_string.replace("{{CURRENT_CONTROLLER}}", gamepad_message);
  }
  {
    Update_Info_p info;
    String current_bitstream;
    OTA_Handler.get_bitstream_update_info(info);
    Bitstream_Handler.get_current_version(current_bitstream);
    page_string.replace("{{LATEST_BS_VERSION}}", info->checked ? info->version : "OTA Server N/A");
    page_string.replace("{{CURRENT_BS_VERSION}}", current_bitstream);
    page_string.replace("{{BS_OTA_HIDDEN}}", !current_bitstream.length() || (info->version.length() && info->version.indexOf(current_bitstream) < 0) ? "" : "hidden");
  }
  {
    Update_Info_p info;  
    esp_app_desc_t app_desc;
    esp_ota_get_partition_description(esp_ota_get_boot_partition(), &app_desc);
    OTA_Handler.get_esp_update_info(info);
    page_string.replace("{{LATEST_ESP_VERSION}}", info->checked ? info->version : "OTA Server N/A");
    page_string.replace("{{CURRENT_ESP_VERSION}}", app_desc.version);
    page_string.replace("{{ESP_OTA_HIDDEN}}", info->version.length() && info->version != String(app_desc.version) ? "" : "hidden");
  }
  {
    String version;
    Mega_Handler.get_update_version(version);

    page_string.replace("{{SHIELD_DETECTED}}", Mega_Handler.get_shield_available() ? "Yes" : "No");
    page_string.replace("{{CURRENT_SHIELD_VERSION}}", version);
    page_string.replace("{{SHIELD_HIDDEN}}", Mega_Handler.get_shield_available() ? "" : "hidden");
  }

  request->send(200, "text/html", page_string);

  lastAccess = esp_timer_get_time();

}

void Web_Handler_Class::handleOTA(AsyncWebServerRequest *request)
{
  String page_string = "";
  Update_Info_p info;
  {
    File page = LittleFS.open("/webpage/ota.html", "r");
    if (page)
    {
      page_string = page.readString();
      page.close();
    }
  }

  if (request->arg("part") == "bitstream")
  {
    page_string.replace("{{UPDATE_PART}}", "bitstream");
    OTA_Handler.get_bitstream_update_info(info);
  }
  else if (request->arg("part") == "esp")
  {
    page_string.replace("{{UPDATE_PART}}", "esp");
    OTA_Handler.get_esp_update_info(info);
  }

  //info.changelog.replace("\r\n", "<br>");
  
  page_string.replace("{{CHANGELOG}}", info->changelog);
  page_string.replace("{{IP_ADDRESS}}", WiFi.localIP().toString());
  page_string.replace("{{VERSION}}", info->version);

  request->send(200, "text/html", page_string);

  lastAccess = esp_timer_get_time();

}

void Web_Handler_Class::handleToken(AsyncWebServerRequest *request)
{
  if (request->hasArg("token"))
  {
    String token = request->arg("token");
    Preferences_Handler.saveOTAToken(token);
    Log_Handler.println("Received token " + token);
  }
  request->redirect("/");

  lastAccess = esp_timer_get_time();

}


void Web_Handler_Class::handleBTReset(AsyncWebServerRequest *request)
{
  BP32.forgetBluetoothKeys();
  request->redirect("/");

  lastAccess = esp_timer_get_time();

}

void Web_Handler_Class::addWebSocket(AsyncWebSocket* handler)
{
  _Aserver.addHandler(handler);
}

String Web_Handler_Class::serial_ip(const String& var)
{
  if (var == "IP_ADDRESS")
  {
    return WiFi.localIP().toString();
  }
  return "";
}

bool Web_Handler_Class::isAccessTimeout(void)
{
    bool retval = false;
    if (esp_timer_get_time() - this->lastAccess > WEB_TIMEOUT)
    {
      retval = true;
    }
    
    return retval;
}

bool Web_Handler_Class::isRunning(void)
{
    return serverRunning;
}

void Web_Handler_Class::shutdown(void)
{
  _Aserver.end();
  serverRunning = false;
}


void Web_Handler_Class::init(void)
{

  // Handle Bitstream upload.
  _Aserver.on("/upgrade/bitstream",  HTTP_POST, handleUploadDone, handleLittleFSFileUpload);
  // Handle ATMega upload.
  _Aserver.on("/upgrade/atmega",     HTTP_POST, handleUploadDone, handleLittleFSFileUpload);

  // // Handle Bitstream upload.
  _Aserver.on("/upgrade/esp32", HTTP_POST, handleUploadDone, handlePartitionUpload);

  // // Handle LittleFS upload.
  _Aserver.on("/upgrade/spiffs", HTTP_POST, handleUploadDone, handlePartitionUpload);

  _Aserver.on("/reboot", HTTP_GET, handleReboot);

  // Handle bt config
  _Aserver.on("/settings.html", HTTP_GET, handleSettings);
  _Aserver.on("/settings.html", HTTP_POST, handleSettings);

  _Aserver.on("/", HTTP_GET, handleIndex);
  _Aserver.serveStatic("/pico.min.css", LittleFS, "/webpage/pico.min.css");
  _Aserver.serveStatic("/gbaHD.css", LittleFS, "/webpage/gbaHD.css");
  _Aserver.serveStatic("/Logo.svg", LittleFS, "/webpage/Logo.svg");
  _Aserver.serveStatic("/serial.html", LittleFS, "/webpage/serial.html").setTemplateProcessor(serial_ip);
  _Aserver.on("/ota.html", HTTP_GET, handleOTA);
  _Aserver.on("/setToken", HTTP_GET, handleToken);
  _Aserver.on("/resetBTDeviceList", HTTP_GET, handleBTReset);

  // Handle everything else.
  _Aserver.onNotFound(_handle404);

  // Set up DNS.
  if (!MDNS.begin("gbahd"))
  {
    Log_Handler.println("Error setting up DNS");
  }

  _Aserver.begin();

  serverRunning = true;

  lastAccess = esp_timer_get_time();

}

void Web_Handler_Class::run(void)
{
  if ((rebootTimer > 0) && (rebootTimer < (esp_timer_get_time() / 1000)))
  {
    ESP.restart();
  }
}
