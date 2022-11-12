



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  //Serial1.begin(9600);
}



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
    //Serial.println(_command + "_" + _payload + "_" + crc16(_command + _payload) + "\n");
    Serial1.println(_command + "_" + _payload + "_" + crc16(_command + _payload) + "\n");
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


void set_clock_time(int _h, int _m, int _s){
  send_cmd_str("st", String(_h) + ":" + String(_m) + ":" + String(_s));
}

void set_brightness(int _b){

}

void set_brightness(bool _set_auto){
  
}


void loop() {
  //send_cmd_str("ct", "1:101:36"); //5650
  //Serial.println(crc16("abc"));
  //delay(100);

   while (Serial1.available()) {
    delay(30); //delay to allow buffer to fill
    if (Serial1.available() > 0) {
      char c = Serial1.read(); //gets one byte from serial buffer
      if (c == '\n') {
        Serial1.flush();
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
      Serial.println(cmd);

    }
  }
  delay(100);
  set_clock_time(19,19,19);
}