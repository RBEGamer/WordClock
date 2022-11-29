#include "wifi_interface.h"

std::string wifi_interface::cmd_rx_buffer;
bool wifi_interface::cmd_rx_complete;
bool wifi_interface::cmd_rx_started;
uint16_t wifi_interface::crc16(const std::string _data, const uint16_t _poly = 0x8408)
{
  uint16_t crc = 0xFFFF;
  for (int i = 0; i < _data.length(); i++)
  {
    const uint8_t b = (uint8_t)_data[i];
    uint8_t cur_byte = 0xFF & b;

    for (int it = 0; it < 8; it++)
    {
      if ((crc & 0x0001) ^ (cur_byte & 0x0001))
      {
        crc = (crc >> 1) ^ _poly;
      }
      else
      {
        crc >>= 1;
      }
      cur_byte >>= 1;
    }
  }
  crc = (~crc & 0xFFFF);
  crc = (crc << 8) | ((crc >> 8) & 0xFF);
  return crc & 0xFFFF;
}

void wifi_interface::enable_uart_irq(bool _irq_state)
{
  // set up and enable the interrupt handlers
  irq_set_exclusive_handler(UART_WIFI_IRQ, wifi_interface::on_wifi_uart_rx);
  irq_set_enabled(UART_WIFI_IRQ, _irq_state);
  // Now enable the UART to send interrupts - RX only
  uart_set_irq_enables(UART_WIFI, _irq_state, false);
}


wifi_interface::rxcmd wifi_interface::manual_uart_rx(){
  std::string tmp = "";
  bool complete = false;
  while (uart_is_readable_within_us(UART_WIFI, 100))
  {
    uint8_t ch = uart_getc(UART_WIFI);
    // READ UNTIL NEW LINE
    if (ch == '\n')
    {
      complete = true;
      printf("manual_uart_rx: %s\n", cmd_rx_buffer.c_str());
      return parse_cmd(tmp, complete);
    }
    else
    {
      tmp += ch;
    }
    sleep_ms(1);
  }

  wifi_interface::rxcmd ret;
  ret.cmdok = false;
  return ret;
}

//TODO STARTCHAR
void wifi_interface::on_wifi_uart_rx()
{
  while (uart_is_readable(UART_WIFI))
  {
    uint8_t ch = uart_getc(UART_WIFI);
    // READ UNTIL NEW LINE
    if (ch == '\n')
    {
      wifi_interface::cmd_rx_complete = true;
      break;
    }
    else
    {
      wifi_interface::cmd_rx_buffer += ch;
    }
  }
}


wifi_interface::rxcmd wifi_interface::parse_cmd(){
  wifi_interface::rxcmd res= parse_cmd(wifi_interface::cmd_rx_buffer, wifi_interface::cmd_rx_complete);
  if(res.cmdok){
    wifi_interface::cmd_rx_buffer = "";
    wifi_interface::cmd_rx_complete = false;
  }
  return res;
}

wifi_interface::rxcmd wifi_interface::parse_cmd(const std::string _cmd_rx_buffer, const bool _cmd_rx_complete)
{
  wifi_interface::rxcmd ret;
  ret.cmdok = false;
  const std::string raw = _cmd_rx_buffer + '_'; // workaround add _ for tokenize
  if (_cmd_rx_complete && raw.length() > 2) //cmd has at least 2 chars
  {
 
    const char *delim = "_";
    std::vector<std::string> out;
    helper::tokenize(raw,  delim, out);
    if (out.size() > 2)
    {

      const std::string cmd = out.at(0);
      const std::string payload = out.at(1);
      const std::string crc= out.at(2);

      // CHECK CRC
      const uint16_t crc_rx = wifi_interface::crc16(cmd + payload);
      // TODO FIX CRC
      if (DISABLE_CRC || crc == std::to_string((int)crc_rx))
      {
        ret.cmdok = true;
        ret.cmd = cmd;
        ret.payload = payload;
      }
      else
      {
        printf("crc mismatch got_crc:-%s- exp_crc:-%i- for cmd:-%s- payload:-%s- orig:-%s-\n", crc.c_str(), crc_rx, cmd.c_str(), payload.c_str(), raw.c_str());
      }
    }else{
      printf("elemtn mismatch for %s  got:%i exp:3 \n", raw.c_str(), out.size());
    }
  }
  return ret;
}

void wifi_interface::init_uart()
{
  uart_init(UART_WIFI, PICO_DEFAULT_UART_BAUDRATE);
  gpio_set_function(PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(PICO_DEFAULT_UART_RX_PIN, GPIO_FUNC_UART);
  uart_set_fifo_enabled(UART_WIFI, true);
}

void wifi_interface::send_cmd_str(const std::string _command, const std::string _payload)
{
  const std::string tosend = _command + "_" + _payload + "_" + std::to_string(wifi_interface::crc16(_command + _payload)) + "\n";
  printf("%s", tosend.c_str());
  uart_puts(UART_WIFI, tosend.c_str());
}

void wifi_interface::send_current_time(const int _h, const int _m, const int _s)
{
  send_cmd_str("ct", std::to_string(_h) + ":" + std::to_string(_m) + ":" + std::to_string(_s));
}

void wifi_interface::send_brightness(const int _b)
{
  send_cmd_str("cb", std::to_string(_b));
}

void wifi_interface::send_temperature(const int _t)
{
  send_cmd_str("ct", std::to_string(_t));
}

void wifi_interface::send_log(const std::string _payload)
{
  send_cmd_str("log", _payload.c_str());
}