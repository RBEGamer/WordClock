//WORDCLOCK WIFI MODUL https://github.com/RBEGamer/WordClock/
//--- CHANGELOG ----
// 16.11.2022 -  INITIAL
// 25.12.2022 -  simplification due to more required parameters
// 25.12.2022 - due to storage working on the rp2040 with firmware V2 and working flash/eeporm storage the wifimodule doesnt save settings locally
// 27.12.2022 - added colormodes and clock reset command
#define VERSION "3.4.1"
// ENABLE ON ESP8266-01 !!!!!!!!!!!!!!!!!!!!!!!!!!
#define USE_LITTLEFS
//FOR ESP8266-01
#define LED_BUILTIN 2

#include <Arduino.h>
#include "FS.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#endif

#ifdef ESP32
#include <WebServer.h>
#define FORMAT_SPIFFS_IF_FAILED true
#ifdef USE_LITTLEFS
#define SPIFFS LITTLEFS
#include <LITTLEFS.h>
#else
#include <SPIFFS.h>
#endif
#include <ESPmDNS.h>
#endif



#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


// CONFIG -------------------------------------
#define WEBSERVER_PORT 80                                     // set the port for the webserver eg 80 8080
#define MDNS_NAME "wordclock"                                 // set hostname
#define WEBSITE_TITLE "Word Clock Wifi Module Configuration"  // name your device
#define SERIAL_BAUD_RATE 9600
#define NTP_SEND_TIME_INTERVAL 5 * 60  //sende zeit an uhr all x minuten
#define DEFAULT_NTP_SERVER "pool.ntp.org"
#define DEFAULT_TIMEZONE 1
// END CONFIG ---------------------------------




String time_last = "not synced";
long last = 0;
long last_ee = 0;
#if defined(ESP8266)
const String BOARD_INFO = "WORDCLOCKWIFIMODULE_FW_" + String(VERSION) + "_BOARD_" + "ESP8266";
#elif defined(ESP32)
const String BOARD_INFO = "WORDCLOCKWIFIMODULE_FW_" + String(VERSION) + "_BOARD_" + "ESP32";
#endif


const String ap_name = "WordClockConfiguration";

// END NEOPIXEL CONF ---------------------------
//FILES FOR STORiNG CONFIGURATION DATA
const char* file_ntp_server = "/file.txt";
const char* file_timezone = "/timezone.txt";
//VARS

int timezone = 0;
String ntp_server_url = "pool.ntp.org";
String last_error = "";
int rtc_hours = 0;
int rtc_mins = 0;
int rtc_secs = 0;
int rtc_day = 1;
int rtc_month = 1;
int rtc_year = 2020;
int rtc_hours_tmp = 0;
//IS SET TO TRUE IN SETUP IF A RTC CLOCK IS DETECTED
//USEDE FOR SOFTWARE CLOCK
unsigned long timeNow = 0;
unsigned long timeLast = 0;


// INSTANCES --------------------------------------------

#ifdef ESP8266
ESP8266WebServer server(WEBSERVER_PORT);
#endif

#ifdef ESP32
WebServer server(WEBSERVER_PORT);
#endif

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, DEFAULT_NTP_SERVER, 0, 60000);
WiFiClient espClient;
WiFiManager wifiManager;






const String phead_1 = "<!DOCTYPE html><html><head><title>";
const String phead_2 = "</title>"
                       "<meta http-equiv='content-type' content='text/html; charset=utf-8'>"
                       "<meta charset='utf-8'>"
                       "<link "
                       "href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/themes/base/"
                       "jquery-ui.css' rel=stylesheet />"
                       "<script "
                       "src='http://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js'></"
                       "script>"
                       "<script "
                       "src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/"
                       "jquery-ui.min.js'></script>"
                       "<style>"
                       "html, body {"
                       "  background: #F2F2F2;"
                       " width: 100%;"
                       " height: 100%;"
                       " margin: 0px;"
                       " padding: 0px;"
                       " font-family: 'Verdana';"
                       " font-size: 16px;"
                       " color: #404040;"
                       " }"
                       "img {"
                       " border: 0px;"
                       "}"
                       "span.title {"
                       " display: block;"
                       " color: #000000;"
                       " font-size: 30px;"
                       "}"
                       "span.subtitle {"
                       " display: block;"
                       " color: #000000;"
                       " font-size: 20px;"
                       "}"
                       ".sidebar {"
                       " background: #FFFFFF;"
                       " width: 250px;"
                       " min-height: 100%;"
                       " height: 100%;"
                       " height: auto;"
                       " position: fixed;"
                       " top: 0px;"
                       " left: 0px;"
                       " border-right: 1px solid #D8D8D8;"
                       "}"
                       ".logo {"
                       " padding: 25px;"
                       " text-align: center;"
                       " border-bottom: 1px solid #D8D8D8;"
                       "}"
                       ".menu {"
                       " padding: 25px 0px 25px 0px;"
                       " border-bottom: 1px solid #D8D8D8;"
                       "}"
                       ".menu a {"
                       " padding: 15px 25px 15px 25px;"
                       " display: block;"
                       " color: #000000;"
                       " text-decoration: none;"
                       " transition: all 0.25s;"
                       "}"
                       ".menu a:hover {"
                       " background: #0088CC;"
                       " color: #FFFFFF;"
                       "}"
                       ".right {"
                       " margin-left: 250px;"
                       " padding: 50px;"
                       "}"
                       ".content {"
                       " background: #FFFFFF;"
                       " padding: 25px;"
                       " border-radius: 5px;"
                       " border: 1px solid #D8D8D8;"
                       "}"
                       "</style>";

const String pstart = "</head>"
                      "<body style='font-size:62.5%;'>"
                      "<div class='sidebar'>"
                      "<div class='logo'>"
                      "<span class='title'>WordCLock - WIFI MODULE</span>"
                      "<span class='subtitle'>- Configuration -</span>"
                      "</div>"
                      "<div class='menu'>"
                      "<a href='index.html'>Settings</a>"
                      "</div>"
                      "</div>"
                      "<div class='right'>"
                      "<div class='content'>";

const String pend = "</div></div></body></html>";





uint16_t crc16(String _data, uint16_t _poly = 0x8408) {
  uint16_t crc = 0xFFFF;
  for (int i = 0; i < _data.length(); i++) {
    const uint8_t b = (byte)_data[i];
    uint8_t cur_byte = 0xFF & b;

    for (int it = 0; it < 8; it++) {
      if ((crc & 0x0001) ^ (cur_byte & 0x0001)) {
        crc = (crc >> 1) ^ _poly;
      } else {
        crc >>= 1;
      }
      cur_byte >>= 1;
    }
  }
  crc = (~crc & 0xFFFF);
  crc = (crc << 8) | ((crc >> 8) & 0xFF);
  return crc & 0xFFFF;
}

void send_cmd_str(const String _command, const String _payload) {
  Serial.println("%" + _command + "_" + _payload + "_" + crc16(_command + _payload));
}

void set_clock_time(const int _h, const int _m, const int _s) {
  send_cmd_str("st", String(_h) + ":" + String(_m) + ":" + String(_s));
}

void set_clock_date(const int _day, const int _month, const int _year) {
  send_cmd_str("sd", String(_day) + ":" + String(_month) + ":" + String(_year));
}

void set_restdefaults(){
  send_cmd_str("res", "1");
}
// ONLY READ THE FIRST LINE UNTIL NEW LINE !!!!!
String read_file(const char* _file, String _default = "") {
  File f = SPIFFS.open(_file, "r");
  String tmp = _default;
  if (!f) {
    last_error = "open filesystem file_ntp_server failed";
  } else {
    tmp = f.readStringUntil('\n');
    last_error = "read from FS:" + String(_file) + " " + tmp;
  }
  return tmp;
}

void restore_eeprom_values() {
  ntp_server_url = read_file(file_ntp_server, DEFAULT_NTP_SERVER);
  timezone = read_file(file_timezone, String(DEFAULT_TIMEZONE)).toInt();
}

bool write_file(const char* _file, String _content) {
  File f = SPIFFS.open(_file, "w");
  if (!f) {
    last_error = "Oeffnen der Datei fehlgeschlagen";
    return -1;
  }
  f.print(_content);
  f.close();
  return 0;
}

void save_values_to_eeprom() {
  write_file(file_ntp_server, ntp_server_url);
  write_file(file_timezone, String(timezone));
}

void write_deffault_to_eeprom() {
  timezone = DEFAULT_TIMEZONE;
  ntp_server_url = DEFAULT_NTP_SERVER;
  save_values_to_eeprom();
  restore_eeprom_values();

  set_restdefaults();
}



uint32_t get_esp_chip_id() {
#if defined(ESP8266)
  return ESP.getChipId();
#elif defined(ESP32)
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return chipId;
#else
  return 0;
#endif
}



void handleSave() {
  // PARSE ALL GET ARGUMENTS
  for (uint8_t i = 0; i < server.args(); i++) {
    // timezone +-12hours
    if (server.argName(i) == "timezone") {
      timezone = server.arg(i).toInt();
      if (timezone > 12 || timezone < -12) {
        timezone = 0;
      }
      last_error = "set timezone to" + String(timezone);
      timeClient.setTimeOffset(timezone * 3600);
      timeClient.forceUpdate();
    } else if (server.argName(i) == "ntp_server_url") {
      ntp_server_url = server.arg(i);
      last_error = "set ntp_server_url to" + ntp_server_url;
      if (ntp_server_url != "") {
        timeClient.setPoolServerName(ntp_server_url.c_str());
        timeClient.forceUpdate();
      }
    } else if (server.argName(i) == "fsformat") {
      if (SPIFFS.format()) {  // formats the filesystem= resets all settings
        last_error = "Datei-System formatiert";
      } else {
        last_error = "Datei-System formatiert Error";
      }
    } else if (server.argName(i) == "eepromread") {
      restore_eeprom_values();  //LOAD CURRENT SAVED DATA
    } else if (server.argName(i) == "factreset") {
      write_deffault_to_eeprom();  //LOAD FACOTRY RESET
    } else if (server.argName(i) == "sendtime") {
      last = 0;
    } else {
      send_cmd_str(server.argName(i), server.arg(i));  //SEND ALL OTHER COMMANDS TO THE CLOCK DIRECTLY
    }
  }
  //SAVE THESE DATA
  timeClient.forceUpdate();
  save_values_to_eeprom();
  last = 0; //TRIGGER SYNC

  server.send(200, "text/html", "<html><head><meta http-equiv='refresh' content='1; url=/' /></head><body>SAVE SETTINGS PLEASE WAIT</body></html>");
}





void handleRoot() {
  String control_forms = "<hr><h2>DEVICE INFO</h2><h3>" + String(MDNS_NAME) + String(get_esp_chip_id()) + "<br><br>" + BOARD_INFO + "</h3><br>";

  control_forms += "<hr><h2>CURRENT NTP TIME</h2><h1>" + String(rtc_hours) + ":" + String(rtc_mins) + ":" + String(rtc_secs) + " " + String(rtc_day) + "." + String(rtc_month) + "." + String(rtc_year) + "</h1>";




                   control_forms += "<br><h3> CLOCK CONTROLS </h3>"
                                    "<form name='btn_off' action='/save' method='GET'>"
                                    "<input type='number' value='"
                                    + String(timezone) + "' name='timezone' min='-12' max='12' required placeholder='1'/>"
                                                         "<input type='submit' value='SET TIMEZONE'/>"
                                                         "</form><br>"
                                                         "<form name='btn_off' action='/save' method='GET'>"
                                                         "<input type='text' value='"
                                    + ntp_server_url + "' name='ntp_server_url' required placeholder='pool.ntp.org'/>"
                                                       "<input type='submit' value='SET NTP SERVER URL'/>"
                                                       "</form><br>";
  "<form name='btn_on' action='/save' method='GET' >"
  "<input type='hidden' value='sendtime' name='sendtime' />"
  "<input type='submit' value='SEND NTP TIME TO CLOCK'/>"
  "</form><br>";
  // FONTFACE
  control_forms += "<p>FONTFACE: </p><form name='btn_on' action='/save' method='GET'><select name='sfp' id='sfp'><option value='0'>GERMAN</option><option value='1'>ENGLISH</option><option value='2'>ITALIAN</option><option value='3'>BELGIAN</option><option value='4'>BINARY</option><option value='5'>DOTS</option><option value='6'>TESTPATTERN</option></select><input type='submit' value='SAVE'/></form><br>";
  //FLIP DISPLAY
  control_forms += "<p>DISPLAY ROTATION: </p><form name='btn_on' action='/save' method='GET'><select name='sdo' id='sdo'><option value='0'>NORMAL</option><option value='1'>ROTATED</option></select><input type='submit' value='SAVE'/></form><br>";
  // DAYLIGHTSAVING
  control_forms += "<p>DAYLIGHTSAVING: </p><form name='btn_on' action='/save' method='GET'><select name='dls' id='dls'><option value='0'>DISBALED</option><option value='1'>ENABLED</option></select><input type='submit' value='SAVE'/></form><br>";
  //BRIGHTNESS
  control_forms += "<p>BRIGHTNESS<br> </p><form name='btn_off' action='/save' method='GET'><input type='number' name='sb' id='sb' min='10' max='255' required placeholder='10-255'/><input type='submit' value='SAVE MANUAL MODE'/></form><br><form name='btn_off' action='/save' method='GET'><input type='hidden' name='sb' id='sb' value='0'/><input type='submit' value='SET AUTO MODE'/></form><br>";
  //BRIGHTNESS CURVE
  control_forms += "<p>BRIGHTNESS CURVE: </p><form name='btn_off' action='/save' method='GET'><input type='number' name='sbc' id='sbc' min='10' max='100' required placeholder='0=LINEAR 10-100=EXPONENTIAL'/><input type='submit' value='SAVE'/></form><br>";
  //COLORMODE
  control_forms += "<p>COLOR MODE: </p><form name='btn_on' action='/save' method='GET'><select name='col' id='col'><option value='0'>RAINBOW SEPARATE</option><option value='1'>RAINBOW EQUAL</option><option value='2'>COLD WHITE</option><option value='3'>WARM WHITE</option></select><input type='submit' value='SAVE'/></form><br>";
  
  //DOTBRIGHTNESS
  control_forms += "<p>DOT BRIGHTNESS: </p><form name='btn_off' action='/save' method='GET'><input type='number' name='dob' id='dob' min='0' max='100' required placeholder='0-100%'/><input type='submit' value='SAVE'/></form><br>";
  // BLINKENDOTS
  control_forms += "<p>BLINKENDOTS: </p><form name='btn_on' action='/save' method='GET'><select name='bld' id='bld'><option value='0'>DISBALED</option><option value='1'>ENABLED</option></select><input type='submit' value='SAVE'/></form><br>";
  
  // CLOCKRESET
  control_forms += "<form name='btn_on' action='/save' method='GET' required ><input type='hidden' value='res' name='res' /><input type='submit' value='CLOCK RESET'/></form><br>";

  // SYSTEM SETTINGS  
  control_forms += "<form name='btn_on' action='/save' method='GET' required ><input type='hidden' value='eepromread' name='eepromread' /><input type='submit' value='READ STORED CONFIG'/></form><br>";
  control_forms += "<form name='btn_on' action='/save' method='GET' required ><input type='hidden' value='fsformat' name='fsformat' /><input type='submit' value='DELETE CONFIGURATION'/></form><br>";
  control_forms += "<form name='btn_on' action='/save' method='GET' required ><input type='hidden' value='factreset' name='factreset' /><input type='submit' value='FACTORY RESET'/></form><br>";
  

  control_forms += "<br><hr><h3>LAST SYSTEM MESSAGE</h3><br>" + last_error;

  server.send(200, "text/html", phead_1 + WEBSITE_TITLE + phead_2 + pstart + control_forms + pend);
}

void handleNotFound() {
  server.send(404, "text/html", "<html><head>header('location: /'); </head></html>");
}



String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}




void display_ip() {
  const String ip = IpAddress2String(WiFi.localIP());
  send_cmd_str("ip", ip);
}


void setup(void) {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("");
  // START THE FILESYSTEM
  if (!SPIFFS.begin()) {
    last_error = "__err_spiffs_init_error_try_to_format__";
    Serial.println(last_error);
    SPIFFS.format();
    if (!SPIFFS.begin()) {
      last_error = "__err_spiffs_format failed__";
      Serial.println(last_error);
    }
  }
  // LOAD SETTINGS
  restore_eeprom_values();
  // START WFIFIMANAGER FOR CAPTIVE PORTAL
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setTimeout(60);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.setBreakAfterConfig(true);
  //DISPLAY IP ON THE CLOCK
  delay(1000);
  if (wifiManager.autoConnect((ap_name + "_" + String(get_esp_chip_id())).c_str())) {
    display_ip();
  }
  // SETUP MDNS
  if (MDNS.begin((MDNS_NAME + String(get_esp_chip_id())).c_str())) {
  }
  //WEBSERVER ROUTES
  delay(1000);
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.on("/index.html", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  //START OTA LIB
  ArduinoOTA.setHostname((MDNS_NAME + String(get_esp_chip_id())).c_str());
  ArduinoOTA.setRebootOnSuccess(true);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_SPIFFS
      type = "filesystem";
    }
    SPIFFS.end();
  });
  ArduinoOTA.onEnd([]() {});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
  ArduinoOTA.onError([](ota_error_t error) {});
  ArduinoOTA.onEnd([]() {
    ESP.restart();
  });
  ArduinoOTA.begin();
  //SETUP  NTP CLIENT
  timeClient.setTimeOffset(timezone);
  timeClient.setPoolServerName(ntp_server_url.c_str());
  timeClient.begin();
  timeClient.setTimeOffset(timezone * 3600);
  timeClient.forceUpdate();
}



bool led_state = false;

void loop(void) {
  //HANDLE SERVER
  server.handleClient();
  //UPDATE NTP NTP
  if ((millis() - last) > (1000 * NTP_SEND_TIME_INTERVAL)) {
    last = millis();
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);
    //HANDLE NTP
    timeClient.update();
    time_last = timeClient.getFormattedTime();
    rtc_hours = timeClient.getHours();
    rtc_mins = timeClient.getMinutes();
    rtc_secs = timeClient.getSeconds();
    // SEND TIME
    set_clock_time(rtc_hours, rtc_mins, rtc_secs);
    // DATE
    time_t epochTime = timeClient.getEpochTime();
    struct tm* ptm = gmtime((time_t*)&epochTime);

    rtc_day = ptm->tm_mday;
    rtc_month = ptm->tm_mon + 1;
    rtc_year = ptm->tm_year + 1900;
    set_clock_date(rtc_day, rtc_month, rtc_year);
  }
  //HANDLE OTA
  ArduinoOTA.handle();
  //DELAY TO HANDLE WIFI
  delay(70);
}
