#include "wifi_interface.h"
#include <string>

uint16_t wifi_interface::crc16(const std::string _data, const uint16_t _poly=0x8408)
{
    uint16_t crc = 0xFFFF;    
    for(int i = 0; i < _data.length(); i++){
      const uint8_t b = (uint8_t)_data[i];
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




void wifi_interface::init_uart()
{
    uart_init(UART_WIFI, PICO_DEFAULT_UART_BAUDRATE);
    gpio_set_function(PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PICO_DEFAULT_UART_RX_PIN, GPIO_FUNC_UART);
}

void wifi_interface::send_cmd_str(const std::string _command, const std::string _payload){
    const std::string tosend = _command + "_" + _payload + "_" + std::to_string(wifi_interface::crc16(_command + _payload)) + "\n";
    printf("%s", tosend.c_str());
    uart_puts(UART_WIFI, tosend.c_str());
}

void wifi_interface::send_current_time(const int _h, const int _m, const int _s){
  send_cmd_str("ct", std::to_string(_h) + ":" + std::to_string(_m) + ":" + std::to_string(_s));
}

void wifi_interface::send_brightness(const int _b){
  send_cmd_str("cb", std::to_string(_b));
}

void wifi_interface::send_temperature(const int _t){
  send_cmd_str("ct", std::to_string(_t));
}

void wifi_interface::send_log(const std::string _payload){
  send_cmd_str("log", _payload.c_str());
}