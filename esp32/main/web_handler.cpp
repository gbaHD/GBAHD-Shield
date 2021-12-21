#include "web_handler.h"
#include <Arduino.h>
#include <SPIFFS.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <Preferences.h>

#include "bitstream_handler.h"


Web_Handler_Class Web_Handler;
WebServer Web_Handler_Class::_server( 80 );
File Web_Handler_Class::fsUpload;

void Web_Handler_Class::_handle404() {
  _server.send( 404, "text/plain", "Not found." );
}

void Web_Handler_Class::_sendOK() {
  _server.send( 200 );
}

void Web_Handler_Class::handleBitStreamUpload() {
  // TBD: Assert size.
  
  // Receive the file and store it into the file system.
  // The SD card should be already mounted.
  HTTPUpload& upload = _server.upload();
  if ( upload.status == UPLOAD_FILE_START ) {
    // Open the file to write.
    //fsUpload = SD_MMC.open( LOADING_DEFAULT_FIEE, "w" );
    fsUpload = SPIFFS.open( BITSTREAM_SPIFFS_PATH, "w");
  } else if ( upload.status == UPLOAD_FILE_WRITE ) {
    if ( fsUpload ) {
      fsUpload.write( upload.buf, upload.currentSize );
    }
  } else if ( UPLOAD_FILE_END == upload.status ) {
    if ( fsUpload ) {
      fsUpload.close();
      Serial.println( "Received file" );
      // Send a response.
      _server.sendHeader( "Location", "/" );
      _server.send( 303 );
    } else {
      _server.send( 500, "text/plain", "500: Error creating file." );
    }
  }
}


void Web_Handler_Class::handleESPUpload() {
  // TBD: Assert size.
  
  // Receive the file and store it into the file system.
  // The SD card should be already mounted.
  HTTPUpload& upload = _server.upload();
  if ( upload.status == UPLOAD_FILE_START ) {
    // Open the file to write.
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
  } else if ( upload.status == UPLOAD_FILE_WRITE ) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
  } else if ( UPLOAD_FILE_END == upload.status ) {
    if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
  }
}

// static void handleWifiConfig(void)
// {
//   String options_string = "";

//   if (WiFi.scanComplete() != WIFI_SCAN_RUNNING)
//   {
//     Serial.print("Starting new scan");
//     WiFi.scanDelete();
//     WiFi.scanNetworks(true);
//   }
//   while(WiFi.scanComplete() == WIFI_SCAN_RUNNING){
//     Serial.print(".");
//     delay(100);
//   }
//   Serial.println("Done!");
//   for (int i = 0; i < WiFi.scanComplete(); ++i) {
//     options_string += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";

//   }

//   File config_template = SPIFFS.open("/webpage/wifi.template", "r");
  
//   String string_template = config_template.readString();

//   config_template.close();

//   string_template.replace("{{SSIDS}}", options_string);
//   if (WiFi.getMode() == WIFI_MODE_AP)
//   {
//     string_template.replace("{{MODE}}", "AP");
//     string_template.replace("{{SSID}}", _ssid);
//     string_template.replace("{{IP}}", WiFi.softAPIP().toString() + "/" + WiFi.softAPSubnetCIDR());
//   }
//   else
//   {
//     string_template.replace("{{MODE}}", "STA");
//     string_template.replace("{{SSID}}", WiFi.SSID());
//     string_template.replace("{{IP}}", WiFi.localIP().toString() + "/" + WiFi.subnetCIDR());
//   }
  
//   _server.sendHeader("Connection", "close");
//   _server.send(200, "text/html", string_template);
  
// }

// static void updateWifiConfig(void)
// {
//   if (_server.args() >= 2)
//   {
//     String ssid;
//     String password;
//     for (uint8_t i = 0U; i < _server.args(); i++)
//     {
//       String arg = _server.argName(i);

//       if (arg.equals("ssid"))
//       {
//         ssid = _server.arg(i);
//       }
//       else if (arg.equals("password"))
//       {
//         password = _server.arg(i);
//       }
//     }

//     Wifi_Handler.updateSTACredentials(ssid, password);
//   }

//   ESP.restart();
  
// }

// static void resetWifiConfig(void)
// {
//   Wifi_Handler.reset();
//   _sendOK();
//   ESP.restart();
// }


void Web_Handler_Class::init(void)
{

  // Handle Bitstream upload.
  _server.on( "/upgrade/bitstream", HTTP_POST, _sendOK, handleBitStreamUpload );

  // Handle Bitstream upload.
  _server.on( "/upgrade/esp32", HTTP_POST, _sendOK, handleESPUpload );

  // _server.on( "/wifi/config", HTTP_GET, handleWifiConfig );
  // _server.on( "/wifi/config", HTTP_POST, updateWifiConfig );
  // _server.on( "/wifi/reset", HTTP_GET, resetWifiConfig );

  _server.serveStatic("/", SPIFFS, "/webpage/index.html");
  _server.serveStatic("/pico.min.css", SPIFFS, "/webpage/pico.min.css");
  _server.serveStatic("/Logo.png", SPIFFS, "/webpage/Logo.png");


  // Handle everything else.
  _server.onNotFound( _handle404 );


  // Set up DNS.
  if ( !MDNS.begin( "gbahd" ) ) {
    Serial.println( "Error setting up DNS" );
  }

  _server.begin();
}



void Web_Handler_Class::run(void)
{
  _server.handleClient();  
}