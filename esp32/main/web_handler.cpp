#include "web_handler.h"
#include <Arduino.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <Preferences.h>


#include "bitstream_handler.h"


#define WIFI_TIMEOUT                ( 100U )
#define WIFI_RECONNECT_THRESHOLD    ( 5U )

#define WIFI_SSID_SIZE    ( 32U )  // 32 length
#define WIFI_PW_SIZE      ( 64U )     // 64 length

// WiFi defs.

static WebServer _server( 80 );
static File fsUpload;
static uint32_t _wifiCounter = 0U;
static uint32_t _wifiReconnectionCounter = 0U;

static bool _softAPEnabled = false;

const char* _ssid = "gbaHD";
const char* _password = "gbahdwifi";

static bool _saveWifiCredentials(String& ssid, String& password)
{
  bool retval = true;


  if ((ssid.length() < WIFI_SSID_SIZE) && (password.length() < WIFI_PW_SIZE))
  {
    Preferences preferences;
    preferences.begin("GBAHD_WIFI");
    preferences.putString("SSID", ssid);
    preferences.putString("PASSWORD", password);
    preferences.end();
    retval = true;
  }
  else
  {
    retval = false;
    Serial.println("Entered SSID or Password is too long and is potentially invalid.");
  }

  
  return retval;
}


static void _restoreWifiCredentials(String& ssid, String& password)
{
  Preferences preferences;
  preferences.begin("GBAHD_WIFI");
  ssid = preferences.getString("SSID", "gbahd");
  password = preferences.getString("PASSWORD", "gbahdwifi");

  preferences.end();
}



static void _handle404() {
  _server.send( 404, "text/plain", "Not found." );
}

static void _sendOK() {
  _server.send( 200 );
}

static void handleBitStreamUpload() {
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


static void handleESPUpload() {
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

static void handleWifiConfig(void)
{
  String options_string = "";

  if (WiFi.scanComplete() != WIFI_SCAN_RUNNING)
  {
    Serial.print("Starting new scan");
    WiFi.scanDelete();
    WiFi.scanNetworks(true);
  }
  while(WiFi.scanComplete() == WIFI_SCAN_RUNNING){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Done!");
  for (int i = 0; i < WiFi.scanComplete(); ++i) {
    options_string += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";

  }

  File config_template = SPIFFS.open("/webpage/wifi.template", "r");
  
  String string_template = config_template.readString();

  config_template.close();

  string_template.replace("{{SSIDS}}", options_string);
  if (_softAPEnabled)
  {
    string_template.replace("{{MODE}}", "AP");
    string_template.replace("{{SSID}}", _ssid);
    string_template.replace("{{IP}}", WiFi.softAPIP().toString() + "/" + WiFi.softAPSubnetCIDR());
  }
  else
  {
    string_template.replace("{{MODE}}", "STA");
    string_template.replace("{{SSID}}", WiFi.SSID());
    string_template.replace("{{IP}}", WiFi.localIP().toString() + "/" + WiFi.subnetCIDR());
  }
  
  _server.sendHeader("Connection", "close");
  _server.send(200, "text/html", string_template);
  
}

static void updateWifiConfig(void)
{
  if (_server.args() >= 2)
  {
    String ssid;
    String password;
    for (uint8_t i = 0U; i < _server.args(); i++)
    {
      String arg = _server.argName(i);

      if (arg.equals("ssid"))
      {
        ssid = _server.arg(i);
      }
      else if (arg.equals("password"))
      {
        password = _server.arg(i);
      }
    }

    _saveWifiCredentials(ssid, password);
  }

  WiFi.softAPdisconnect();
  ESP.restart();
  
}

static void resetWifiConfig(void)
{
  String empty = "";
  _saveWifiCredentials(empty, empty);
  _sendOK();
  ESP.restart();
}

static void createWifiAP(void)
{
    WiFi.disconnect();

    _softAPEnabled = WiFi.softAP(_ssid, _password);
}


static void updateWifiStatus(void)
{
  static wl_status_t _lastStatus = WL_DISCONNECTED;

  _softAPEnabled = (WiFi.getMode()  & WIFI_MODE_AP) != 0;
  wl_status_t _newStatus = WiFi.status();
  String _debugString = "";

  switch (_newStatus)
  { 
    case WL_CONNECTED:
      if (_softAPEnabled)
      {
        _wifiCounter = 0U;
        _softAPEnabled = !WiFi.softAPdisconnect();
      }
      break;
    case WL_IDLE_STATUS:
      Serial.println("WiFi: Idle");
      break;
    case WL_SCAN_COMPLETED:
      Serial.println("WiFi: Scan completed");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("WiFi: No SSID");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("WiFi: Connect failed!");
      if ((!_softAPEnabled) && (_wifiCounter > WIFI_TIMEOUT) )
      {      
        Serial.println("Connect failed, setting up AP Mode");
        createWifiAP();
      }
      else
      {
        _wifiCounter++;
      }
      break;
    case WL_CONNECTION_LOST:
      Serial.println("WiFi: Connection lost");
      if (_wifiReconnectionCounter < WIFI_TIMEOUT)
      {
        WiFi.reconnect();
      }
      else
      {
        createWifiAP();
      }
      break;
    case WL_DISCONNECTED:
      if (!_softAPEnabled)
      {
        Serial.println("WiFi: Disconnected");
      }
      else
      {
        Serial.println("WiFi: SoftAP");
      }
      break;
    default:
      if (!_softAPEnabled)
      {
        Serial.println(WiFi.status());
      }
     break;
  }
}

void web_handler_init(void)
{
  String ssid;
  String password;
  
  _restoreWifiCredentials(ssid, password);

  // Setup the WiFi.
  WiFi.mode(WIFI_AP_STA);  
  WiFi.setHostname("gbaHD");
  WiFi.begin(ssid.c_str(), password.c_str());    


  // Handle Bitstream upload.
  _server.on( "/upgrade/bitstream", HTTP_POST, _sendOK, handleBitStreamUpload );

  // Handle Bitstream upload.
  _server.on( "/upgrade/esp32", HTTP_POST, _sendOK, handleESPUpload );

  _server.on( "/wifi/config", HTTP_GET, handleWifiConfig );
  _server.on( "/wifi/config", HTTP_POST, updateWifiConfig );
  _server.on( "/wifi/reset", HTTP_GET, resetWifiConfig );

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



void web_handler_run(void)
{
  _server.handleClient();
  updateWifiStatus();
  
}