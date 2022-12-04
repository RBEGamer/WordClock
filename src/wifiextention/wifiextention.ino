//FH AACHEN WORDCLOCK WIFI MODULW https://github.com/RBEGamer/WordClock/
//--- CHANGELOG ----
// 16.11.2022 -  INITIAL

#define VERSION "1.0"

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





#if defined(ESP32)
#include "TZ.h"
#elif defined(ESP8266)
#include <TZ.h>
#else
#include "TZ.h"
#endif


#define MYTZ TZ_Europe_London



// CONFIG -------------------------------------
#define WEBSERVER_PORT 80 // set the port for the webserver eg 80 8080
#define MDNS_NAME "wordclock" // set hostname
#define WEBSITE_TITLE "Word Clock Wifi Module Configuration" // name your device
#define SERIAL_BAUD_RATE 9600
#define NTP_SEND_TIME_INTERVAL 5*60 //sende zeit an uhr all x minuten
#define DEFAULT_NTP_SERVER "pool.ntp.org"
#define DEFAULT_TIMEZONE 1
#define DEFAULT_BEIGHTNESS 255
#define DEFAULT_DLS 0








// END CONFIG ---------------------------------


String time_last = "not synced";
long last = 0;

#if defined(ESP8266)
const String BOARD_INFO = "WORDCLOCKWIFIMODULE_FW_" + String(VERSION) + "_BOARD_" + "ESP8266";
#elif defined(ESP32)
const String BOARD_INFO= "WORDCLOCKWIFIMODULE_FW_" + String(VERSION) + "_BOARD_" + "ESP32";
#endif


const String ap_name = "WordClockConfiguration";

// END NEOPIXEL CONF ---------------------------
//FILES FOR STORiNG CONFIGURATION DATA
const char* file_ntp_server = "/file.txt";
const char* file_timezone = "/timezone.txt";
const char* file_brightness = "/brightness.txt";
const char* file_dalight_saving_enabled = "/dls.txt";
//VARS

int timezone = 0;
int brightness = 255;
int fontface = 0;
int flip_display = 0;
String ntp_server_url = "pool.ntp.org";

int rtc_hours = 0;
int rtc_mins = 0;
int rtc_secs = 0;
int rtc_day = 1;
int rtc_month = 1;
int rtc_year = 2020;
int rtc_hours_tmp = 0;


int dalight_saving_enabled = 0;
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
NTPClient timeClient(ntpUDP,DEFAULT_NTP_SERVER,0,60000);
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

const String pend = "</div>"
                    "</div>"
                    "</body>"
                    "</html>";

String last_error = "";



uint16_t crc16(String _data, uint16_t _poly=0x8408)
{
    uint16_t crc = 0xFFFF;    
    for(int i = 0; i < _data.length(); i++){
      const uint8_t b = (byte)_data[i];
      uint8_t cur_byte = 0xFF & b;
      
      for(int it = 0; it < 8; it++){
        if((crc & 0x0001) ^ (cur_byte & 0x0001)){
          crc = (crc >> 1) ^ _poly;
        }else{
          crc >>= 1; 
        }
        cur_byte >>= 1;
      }
    }
    crc = (~crc & 0xFFFF);
    crc = (crc << 8) | ((crc >> 8) & 0xFF);
    return crc & 0xFFFF;
}

void send_cmd_str(String _command, String _payload){
    Serial.println("%"+_command + "_" + _payload + "_" + crc16(_command + _payload) + "\n");
}



String readString;
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void set_clock_time(const int _h, const int _m, const int _s){
  send_cmd_str("st", String(_h) + ":" + String(_m) + ":" + String(_s));
}


int last_set_brightness = 255;
void set_brightness(int _b){
  if(_b <= 0){send_cmd_str("sb", "0");return;}
  if(_b < 10){_b = 10;}
  else if(_b > 255){_b = 255;}
  send_cmd_str("sb", String(_b));
  last_set_brightness = _b;
}

void set_brightness(bool _set_auto){
  if(_set_auto){
    send_cmd_str("sb", "0");
  }else{
   set_brightness(last_set_brightness);
  }
  
}


void set_flip_display(bool _set_flip){
  if(_set_flip){
    send_cmd_str("fd", "0");
  }else{
    send_cmd_str("fd", "0");
  }
}

void set_fontface(int _fontface_id){
    send_cmd_str("fd", String(abs(_fontface_id)));
}


// ONLY READ THE FIRST LINE UNTIL NEW LINE !!!!!
String read_file(const char* _file, String _default = "")
{
    File f = SPIFFS.open(_file, "r");
    String tmp = _default;
    if (!f) {
        last_error = "open filesystem file_ntp_server failed";
    }
    else {
        tmp = f.readStringUntil('\n');
        last_error = "read from FS:" + String(_file) + " " + tmp;
    }
    return tmp;
}

void restore_eeprom_values()
{
    ntp_server_url = read_file(file_ntp_server,DEFAULT_NTP_SERVER);
    timezone = read_file(file_timezone, String(DEFAULT_TIMEZONE)).toInt();
    brightness = read_file(file_brightness, String(DEFAULT_BEIGHTNESS)).toInt();
    dalight_saving_enabled = read_file(file_dalight_saving_enabled, String(DEFAULT_DLS)).toInt();
}

bool write_file(const char* _file, String _content)
{
    File f = SPIFFS.open(_file, "w");
    if (!f) {
        last_error = "Oeffnen der Datei fehlgeschlagen";
        return -1;
    }
    f.print(_content);
    f.close();
    return 0;
}

void save_values_to_eeprom()
{
  write_file(file_ntp_server, ntp_server_url);
  write_file(file_timezone, String(timezone));
  write_file(file_brightness, String(brightness));
  write_file(file_dalight_saving_enabled, String(dalight_saving_enabled));
}


void write_deffault_to_eeprom(){
  timezone = DEFAULT_TIMEZONE;
  ntp_server_url = DEFAULT_NTP_SERVER;
  brightness = DEFAULT_BEIGHTNESS;
  dalight_saving_enabled = DEFAULT_DLS;          
  save_values_to_eeprom();
}





//https://forum.arduino.cc/t/time-libary-sommerzeit-winterzeit/221884/2
boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
  if (month<3 || month>10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (month>3 && month<10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (month==3 && (hour + 24 * day)>=(1 + tzHours + 24*(31 - (5 * year /4 + 4) % 7)) || month==10 && (hour + 24 * day)<(1 + tzHours + 24*(31 - (5 * year /4 + 1) % 7)))
    return true;
  else
    return false;
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











 
void handleSave()
{
    // PARSE ALL GET ARGUMENTS
    for (uint8_t i = 0; i < server.args(); i++) {
        // timezone +-12hours
        if (server.argName(i) == "timezone") {
            timezone = server.arg(i).toInt();
            if (timezone > 12 || timezone < -12) {
                timezone = 0;
            }
            last_error = "set timezone to" + String(timezone);
            timeClient.setTimeOffset(timezone*3600);
            timeClient.forceUpdate();
        }
        // ntp_server_url
        if (server.argName(i) == "ntp_server_url") {
            ntp_server_url = server.arg(i);
            last_error = "set ntp_server_url to" + ntp_server_url;
            if(ntp_server_url != ""){
              timeClient.setPoolServerName(ntp_server_url.c_str());
              timeClient.forceUpdate();
            }
        }     

        if (server.argName(i) == "brightness") {
            brightness = server.arg(i).toInt();
            set_brightness(brightness);
            last_error = "set brightness to" + String(brightness);
        }
        


        if (server.argName(i) == "fd") {
            flip_display = server.arg(i).toInt();
            set_flip_display((bool)flip_display);
            last_error = "set set_flip_display to" + String(flip_display);
        }

        if (server.argName(i) == "fp") {
            fontface = server.arg(i).toInt();
            set_fontface(fontface);
            last_error = "set set_fontface to" + String(fontface);
        }




        if (server.argName(i) == "dalight_saving_enabled") {
            dalight_saving_enabled = server.arg(i).toInt();
            last_error = "set dalight_saving_enabled to" + dalight_saving_enabled;
        }  
        // formats the filesystem= resets all settings
        if (server.argName(i) == "fsformat") {
            if (SPIFFS.format()) {
                last_error = "Datei-System formatiert";
            }
            else {
                last_error = "Datei-System formatiert Error";
            }          
        }
        //LOAD CURRENT SAVED DATA
        if (server.argName(i) == "eepromread") {
            restore_eeprom_values();
        }

        //LOAD FACOTRY RESET
        if (server.argName(i) == "factreset") {
           write_deffault_to_eeprom();
        }
      if (server.argName(i) == "sendtime") {
            last = 0;
            delay(100);
        }  
    }
    //SAVE THESE DATA
    timeClient.forceUpdate();
    save_values_to_eeprom();
    last = 0;
    
    server.send(300, "text/html","<html><head><meta http-equiv='refresh' content='1; url=/' /></head><body>SAVE SETTINGS PLEASE WAIT</body></html>");
}






void handleRoot()
{



    String control_forms = "<hr><h2>DEVICE INFO</h2>";
    control_forms+="<h3>" + String(MDNS_NAME) + String(get_esp_chip_id()) + "<br><br>"+BOARD_INFO+"</h3><br>";



  
    String timezonesign = "";
    if(timezone > 0){
      timezonesign = "+";
    }
   
 
     control_forms+="<hr><h2>CURRENT TIME</h2><h1>" + String(rtc_hours) + ":"+ String(rtc_mins) + ":" + String(rtc_secs) +"</h1>"
                    "<hr><h2>LAST NTP TIME</h2><h1>" + time_last + " ("+timezonesign+" "+ String(timezone)+" Hours)</h1><br>"
                    "<h2>LAST NTP DATE</h2><h1>" + String(rtc_day) + "."+String(rtc_month)+"."+ String(rtc_year)+"</h1>";





    control_forms += "<hr><h2>CONTROLS</h2>";

    
    
                     


     control_forms += "<br><h3> CLOCK CONTROLS </h3>"
                      "<form name='btn_off' action='/save' method='GET'>"
                     "<input type='number' value='"+ String(timezone) + "' name='timezone' min='-12' max='12' required placeholder='1'/>"
                     "<input type='submit' value='SET TIMEZONE'/>"
                     "</form><br>"
                     "<form name='btn_off' action='/save' method='GET'>"
                     "<input type='text' value='"+ ntp_server_url + "' name='ntp_server_url' required placeholder='pool.ntp.org'/>"
                     "<input type='submit' value='SET NTP SERVER URL'/>"
                     "</form><br>";
                     "<form name='btn_offmq' action='/save' method='GET'>"
                     "<input type='number' min='0' max='1' value='"+ String(dalight_saving_enabled) + "' name='dalight_saving_enabled' required placeholder='0'/>"
                     "<input type='submit' value='ENABLE GERMAN DAYLIGHT SAVING'/>"
                     "</form>"
                     "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='sendtime' name='sendtime' />"
                     "<input type='submit' value='SEND NTP TIME TO CLOCK'/>"
                     "</form><br>";

    control_forms += "<form name='btn_on' action='/save' method='GET' required ><select name='fp' id='fp'>";

                         if(fontface == 0){
                            control_forms += "<option value='0' selected>GERMAN</option>";
                         }else{
                            control_forms += "<option value='0'>GERMAN</option>";
                         }
                         if(fontface == 1){
                            control_forms += "<option value='1' selected>ENGLISH</option>";
                         }else{
                            control_forms += "<option value='1'>ENGLISH</option>";
                         }
                         if(fontface == 2){
                            control_forms += "<option value='2' selected>BINARY</option>";
                         }else{
                            control_forms += "<option value='2'>BINARY</option>";
                         }
                         if(fontface == 3){
                            control_forms += "<option value='2' selected>DOTS</option>";
                         }else{
                            control_forms += "<option value='2'>DOTS</option>";
                         }

    control_forms += "<form name='btn_on' action='/save' method='GET' required ><select name='fd' id='fd'>";

                         if(flip_display == 0){
                            control_forms += "<option value='0' selected>NORMAL</option>";
                         }else{
                            control_forms += "<option value='0'>NORMAL</option>";
                         }
                         if(flip_display == 1){
                            control_forms += "<option value='1' selected>ROTATED</option>";
                         }else{
                            control_forms += "<option value='1'>ROTATED</option>";
                         }                     
                     
        control_forms += "</select><input type='submit' value='SAVE'/></form>";


     control_forms += "<br><h3>OTHER SETTINGS </h3>"
                    "<form name='btn_off' action='/save' method='GET'>"
                     "<input type='number' value='"+ String(brightness) + "' name='brightness' min='0' max='255' required placeholder='0=AUTO'/>"
                     "<input type='submit' value='SET BRIGHTNESS'/>"
                     "</form><br><br><br>"
                     "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='eepromread' name='eepromread' />"
                     "<input type='submit' value='READ STORED CONFIG'/>"
                     "</form><br>"
                     "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='fsformat' name='fsformat' />"
                     "<input type='submit' value='DELETE CONFIGURATION'/>"
                     "</form><br>"
                     "<form name='btn_on' action='/save' method='GET' required >"
                     "<input type='hidden' value='factreset' name='factreset' />"
                     "<input type='submit' value='FACTORY RESET'/>"
                     "</form><br>";



    control_forms += "<br><hr><h3>LAST SYSTEM MESSAGE</h3><br>" + last_error;

    server.send(200, "text/html", phead_1 + WEBSITE_TITLE + phead_2 + pstart + control_forms + pend);
}



void handleNotFound()
{
    server.send(404, "text/html","<html><head>header('location: /'); </head></html>");
}



String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}




void display_ip(){
  const String ip = IpAddress2String(WiFi.localIP());
      Serial.println("_ip_" + ip + "_");
      for(int i =0; i < strlen(ip.c_str()); i++ ) {
        
        if(ip[i] == '.'){
          //update_clock_display(0,0,0,128,64,false); //DISPLAY WIFI ERROR
        }else{
          //update_clock_display(ip[i]-'0',0,0,0,255,false); //DISPLAY WIFI ERROR
         // Serial.println("_st_" + String(ip[i]-'0') + "_0_");
        }
        delay(1000);
      }
  }

  
void setup(void)
{ 


    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("_setup_started_");
    // START THE FILESYSTEM
    int fsok = SPIFFS.begin();


      if(fsok){
        
      }
      else {
        last_error = "__err_spiffs_init_error_try_to_format__";
        Serial.println(last_error);
        SPIFFS.format();
        
        fsok = SPIFFS.begin();
        if(!fsok){
          last_error = "__err_spiffs_format failed__";
          Serial.println(last_error);
         }
          
      }

    
    // LOAD SETTINGS
    restore_eeprom_values();


    // START WFIFIMANAGER FOR CAPTIVE PORTAL
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(true);
    wifiManager.setTimeout(60);
    wifiManager.setConfigPortalTimeout(180);
   // wifiManager.setAPClientCheck(true);
    wifiManager.setBreakAfterConfig(true);
   // wifiManager.setClass("invert"); 
    //TRY TO CONNECT
    // AND DISPLAY IP ON CLOCKS HOUR DISPLAY (FOR 2 DIGIT CLOCKS)
  
    delay(1000);
    if(wifiManager.autoConnect(( ap_name +"_" + String(get_esp_chip_id())).c_str())){
      display_ip();
      
    }else{
    }

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
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        }
        else { // U_SPIFFS
            type = "filesystem";
        }
        SPIFFS.end();
    });
    ArduinoOTA.onEnd([]() {});
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
    ArduinoOTA.onError([](ota_error_t error) {});
    ArduinoOTA.onEnd([]() {
        //if (SPIFFS.begin(true)) {
       //     restore_eeprom_values(); // RESTORE FILE SETTINGS
       // }
       ESP.restart();
    });
    ArduinoOTA.begin();


    //SETUP  NTP CLIENT
    timeClient.setTimeOffset(timezone);
    timeClient.setPoolServerName(ntp_server_url.c_str());
    timeClient.begin();

    timeClient.setTimeOffset(timezone*3600);
    timeClient.forceUpdate();

        
  //Serial.println("_setup_complete_");

}



bool led_state = false;


void loop(void)
{

    //HANDLE SERVER
    server.handleClient();

    

 
    //UPDATE NTP NTP
   
    if ((millis() - last) > (1000*NTP_SEND_TIME_INTERVAL)) {
      last = millis();  
      led_state = !led_state;
      digitalWrite(LED_BUILTIN, led_state); 
      //HANDLE NTP
      timeClient.update();
      time_last = timeClient.getFormattedTime(); 
      rtc_hours = timeClient.getHours();
      rtc_mins = timeClient.getMinutes();
      rtc_secs = timeClient.getSeconds();
      //CHECK SUMMERTIME IF ENABLED
      if(dalight_saving_enabled && summertime_EU(rtc_year, rtc_month, rtc_day, rtc_hours,0)){
       rtc_hours_tmp = rtc_hours + 1;
       if(rtc_hours_tmp >= 24){
         rtc_hours_tmp = (rtc_hours_tmp - 24);
       }
      }
      set_clock_time(rtc_hours, rtc_mins, rtc_secs);
    }

 
    //HANDLE COMMANDS FROM THE CLOCK
    // PARSE GOT CLOCK INFORMATION
   while (Serial.available()) {
    delay(30); //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read(); //gets one byte from serial buffer
      if (c == '\n') {
        Serial.flush();
        break;
      }else{
        readString += c;
      }
    }
  }

  if (readString.length() > 0) {
    const String cmd = getValue(readString, '_', 0);
    const String payload = getValue(readString, '_', 1);
    const String crc = getValue(readString, '_', 2);
    readString = "";

    // CHECK CRC
    if(crc == String(crc16(cmd + payload))){
      last_error = readString;
      if(cmd == "ct"){

      }else if(cmd == "cb"){

      }

    }
  }
   
 
    //HANDLE OTA
    ArduinoOTA.handle();
   
 
    delay(70);
}
