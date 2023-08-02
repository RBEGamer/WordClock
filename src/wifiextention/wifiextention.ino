//WORDCLOCK WIFI MODUL https://github.com/RBEGamer/WordClock/
//--- CHANGELOG ----
// 16.11.2022 -  INITIAL
// 25.12.2022 -  simplification due to more required parameters
// 25.12.2022 - due to storage working on the rp2040 with firmware V2 and working flash/eeporm storage the wifimodule doesnt save settings locally
// 27.12.2022 - added colormodes and clock reset command
#define VERSION "4.0.0"
// ENABLE ON ESP8266-01 !!!!!!!!!!!!!!!!!!!!!!!!!!
#define USE_LITTLEFS
//FOR ESP8266-01
#define LED_BUILTIN 2
#include <ESPUI.h>
#include <Arduino.h>
#include "FS.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#//include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#endif

#ifdef ESP32
//#include <WebServer.h>
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
#define WEBSITE_TITLE "Word Clock Configuration"  // name your device
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

bool force_ntp_update = true;

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
//ESP8266WebServer server(WEBSERVER_PORT);
#endif

#ifdef ESP32
//WebServer server(WEBSERVER_PORT);
#endif

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, DEFAULT_NTP_SERVER, 0, 60000);
WiFiClient espClient;
WiFiManager wifiManager;








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
  send_cmd_str("sd", String(_day) + "." + String(_month) + "." + String(_year));
}

void request_command_lut() {
  send_cmd_str("syssc", "");
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
  //server.on("/", handleRoot);
  //server.on("/save", handleSave);
  //server.on("/index.html", handleRoot);
  //server.onNotFound(handleNotFound);
  //server.begin();
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

  
  ESPUI.begin(WEBSITE_TITLE, "wordclock", "wordclock");
  ESPUI.label("BOARD INFO:", ControlColor::Turquoise, BOARD_INFO);
}



bool led_state = false;

void loop(void) {
  //HANDLE SERVER
  //server.handleClient();
  //UPDATE NTP NTP
  if ((millis() - last) > (1000 * NTP_SEND_TIME_INTERVAL) || force_ntp_update) {

    if(force_ntp_update){
        request_command_lut();
        delay(100);
        force_ntp_update = false;
    }

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
    delay(100);
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
