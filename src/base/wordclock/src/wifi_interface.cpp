#include "wifi_interface.h"

bool wifi_interface::register_rx_callback(const std::function<void(const std::string _payload)> _callback, const wifi_interface::CMD_INDEX _cmd)
{
  if (_cmd == wifi_interface::CMD_INDEX::LENGHT)
  {
    return false;
  }
  if (_callback != nullptr)
  {
    callback_setup = true;
    wifi_interface::RX_CALLBACK_FUNCTIONS[wifi_interface::CMD_LUT[(int)_cmd]] = _callback;
  }

  return true;
}

#ifdef ENABLE_CRC
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
#endif

void wifi_interface::enable_uart_irq(bool _irq_state)
{
  const int UART_IRQ = UART_WIFI == uart0 ? UART0_IRQ : UART1_IRQ;
  // set up and enable the interrupt handlers
  irq_set_exclusive_handler(UART_IRQ, wifi_interface::on_wifi_uart_rx);
  irq_set_enabled(UART_IRQ, _irq_state);
  // Now enable the UART to send interrupts - RX only
  uart_set_irq_enables(UART_WIFI, _irq_state, false);
  // DIABLE FIFO IN IRQ MODE
  // FIFO IS A BITT BUGGY IN IRQ
  // SO SWITCHTING TO SINGLE CHARACTER IRQ
  if (_irq_state)
  {
    uart_set_fifo_enabled(UART_WIFI, false);
  }
  else
  {
    uart_set_fifo_enabled(UART_WIFI, true);
  }
}

void wifi_interface::on_wifi_uart_rx()
{
  // WAIT FOR A MORE BYTES UP TO 10ms
  while (uart_is_readable(UART_WIFI))
  {
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    const char ch = uart_getc(UART_WIFI);
    // READ UNTIL NEW LINE
    if (ch == '\n')
    {
      wifi_interface::rx_recieved_queue.push(wifi_interface::cmd_rx_buffer);
      wifi_interface::cmd_rx_buffer = "";
      wifi_interface::cmd_started = false;
    }
    else if (ch == CMD_START_CHARACTER && !wifi_interface::cmd_started)
    {
      wifi_interface::cmd_rx_buffer = "";
      wifi_interface::cmd_started = true;
    }
    else if (wifi_interface::cmd_started)
    {
      wifi_interface::cmd_rx_buffer += ch;
    }
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    sleep_ms(1);
  }
}

void wifi_interface::process_cmd()
{
  // PROCESS
  if (wifi_interface::rx_recieved_queue.size() > 0)
  {
    const std::string front_cmd = wifi_interface::rx_recieved_queue.front();
    wifi_interface::rx_recieved_queue.pop();
    process_cmd_str(front_cmd);
  }
}

wifi_interface::rxcmd wifi_interface::process_cmd_str(const std::string _rx_buffer)
{

  wifi_interface::rxcmd res = check_extract_cmd(_rx_buffer);
  // paqrsed cmd has the right lenght and seperators and crc matches
  // so proceed with calling the right function
  if (res.cmdok)
  {
    // IF SETUP CALL CALLBACKFKT
    if (wifi_interface::callback_setup)
    {
      // MAP CMD INDEX ENUM TO STRING VERSERION  CMD_INDEX::LOG => "log"
      const std::string cmd_str = wifi_interface::CMD_LUT[(int)res.cmd];
      // FIND ELEMENT STring_VERSION THAT MATCHES THE STRING and CAll SET FUNCTION
      if (wifi_interface::RX_CALLBACK_FUNCTIONS.find(cmd_str) != wifi_interface::RX_CALLBACK_FUNCTIONS.end())
      {
        // FINALLY CALL FUNCTION
        wifi_interface::RX_CALLBACK_FUNCTIONS[cmd_str](res.payload);
      }
      else
      {
        printf("parsecmd callback %s not found", wifi_interface::CMD_LUT[(int)res.cmd]);
      }
    }
  }
  return res;
}

wifi_interface::rxcmd wifi_interface::check_extract_cmd(const std::string _cmd_rx_buffer)
{
  wifi_interface::rxcmd ret;
  ret.cmdok = false;
  const std::string raw = _cmd_rx_buffer + '_'; // workaround add _ for tokenize
  if (raw.length() > 2)                         // cmd has at least 2 chars
  {

    const char *delim = "_";
    std::vector<std::string> out;
    helper::tokenize(raw, delim, out);
    if (out.size() > 2)
    {

      const std::string cmd = out.at(0);
      const std::string payload = out.at(1);
      const std::string crc = out.at(2);

      // CHECK CRC
      // TODO FIX CRC ON ESP8266 SIDE
#ifdef ENABLE_CRC
      const uint16_t crc_rx = wifi_interface::crc16(cmd + payload);
      if (crc == std::to_string((int)crc_rx))
      {
#else
      if (true)
      {
#endif
        ret.cmdok = true;
        ret.cmd = CMD_INDEX::INVALID;
        for (int i = 0; i < ((int)wifi_interface::CMD_INDEX::LENGHT); i++)
        {
          const std::string cmd_l = wifi_interface::CMD_LUT[i];
          if (cmd == cmd_l)
          {
            ret.cmd = static_cast<wifi_interface::CMD_INDEX>(i);
          }
        }
        ret.payload = payload;
      }
      else
      {
#ifdef ENABLE_CRC
        printf("crc mismatch got_crc:-%s- exp_crc:-%i- for cmd:-%s- payload:-%s- orig:-%s-\n", crc.c_str(), crc_rx, cmd.c_str(), payload.c_str(), raw.c_str());
#endif
      }
    }
    else
    {
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

void wifi_interface::send_cmd_str(const wifi_interface::CMD_INDEX _cmd, const std::string _payload)
{

  if (_cmd > wifi_interface::CMD_INDEX::LENGHT)
  {
    return;
  }

  const std::string command = CMD_LUT[(int)_cmd];
#ifdef ENABLE_CRC
  const std::string tosend = CMD_START_CHARACTER + command + "_" + _payload + "_" + std::to_string(wifi_interface::crc16(command + _payload)) + "\n";
#else
  const std::string tosend = CMD_START_CHARACTER + command + "_" + _payload + "_4242\n";
#endif

  printf("%s", tosend.c_str());
  uart_puts(UART_WIFI, tosend.c_str());
}

void wifi_interface::send_time(const int _h, const int _m, const int _s)
{
  send_cmd_str(wifi_interface::CMD_INDEX::TIME, std::to_string(_h) + ":" + std::to_string(_m) + ":" + std::to_string(_s));
}

void wifi_interface::send_date(const int _day, const int _month, const int _year)
{
  send_cmd_str(wifi_interface::CMD_INDEX::DATE, std::to_string(_day) + ":" + std::to_string(_month) + ":" + std::to_string(_year));
}

void wifi_interface::send_brightness(const int _b)
{
  send_cmd_str(wifi_interface::CMD_INDEX::BRIGHTNESS, std::to_string(_b));
}

void wifi_interface::send_brightnesscurve(const int _bc)
{
  send_cmd_str(wifi_interface::CMD_INDEX::BRIGHTNESSCURVE, std::to_string(_bc));
}

void wifi_interface::send_log(const std::string _payload)
{
  send_cmd_str(wifi_interface::CMD_INDEX::LOG, _payload.c_str());
}

void wifi_interface::send_daylightsaving(const bool _dls)
{
  send_cmd_str(wifi_interface::CMD_INDEX::DAYLIGHTSAVING, std::to_string((int)_dls));
}