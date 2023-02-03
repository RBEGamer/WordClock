#include "firmware.hpp"

const bool firmware::flash()
{

#ifndef INCLUDE_ESP_FIRMWARE
    return false;
#endif

#ifndef FIRMWARE_LEN
    return false;
#endif

#if FIRMWARE_LEN <= 0
    return false;
#endif

#if defined(FIRMWARE_LEN) && defined(INCLUDE_ESP_FIRMWARE)
    firmware::prepare_gpio();

    // SAVE CURRENT UART SETTINGS

    // RECONFIG UART
    uart_deinit(UART_FWUPDATE);

    // PERFORM ENTER BOOTMODE
    firmware::trigger_flashmode();

    // SEND INITIAL PAYLOAD

    // RESTORE UART
    uart_deinit(UART_FWUPDATE);
    uart_init(UART_FWUPDATE, UART_FWUPDATE_BAUDRATE);
    gpio_set_function(PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PICO_DEFAULT_UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_fifo_enabled(UART_FWUPDATE, true);

    // PERFORM RESET
    firmware::trigger_reset();

    firmware::reset_gpio();

#endif
    return false;
}

const void firmware::prepare_gpio()
{
    // CONFIGURE GPIOs
    gpio_init(ESP_RST_GPIO);
    gpio_set_dir(ESP_RST_GPIO, GPIO_OUT);
    gpio_put(ESP_RST_GPIO, true);

    gpio_init(ESP_BOOT_GPIO);
    gpio_set_dir(ESP_BOOT_GPIO, GPIO_OUT);
    gpio_put(ESP_BOOT_GPIO, true);
}

const void firmware::trigger_flashmode()
{
    // HOLD BOOT AND RESET
    gpio_put(ESP_RST_GPIO, false);
    gpio_put(ESP_BOOT_GPIO, false);
    sleep_ms(500);
    // HOLD BOOT RELEASE RESET
    gpio_put(ESP_RST_GPIO, true);
    gpio_put(ESP_BOOT_GPIO, false);
    sleep_ms(1000);
    // RELEASE BOOT
    gpio_put(ESP_RST_GPIO, true);
    gpio_put(ESP_BOOT_GPIO, true);
    sleep_ms(500);
}

const void firmware::reset_gpio()
{
    // RECONFIG GPIO TO INPUTS
    gpio_set_dir(ESP_RST_GPIO, GPIO_IN);
    gpio_put(ESP_RST_GPIO, false);

    gpio_set_dir(ESP_BOOT_GPIO, GPIO_IN);
    gpio_put(ESP_BOOT_GPIO, false);
}

const bool firmware::trigger_reset()
{
    // PERFORM RESET
    sleep_ms(1000);
    gpio_put(ESP_RST_GPIO, false);
    sleep_ms(500);
    gpio_put(ESP_RST_GPIO, true);
    return true;
}

const bool firmware::reset()
{
    firmware::prepare_gpio();
    firmware::trigger_reset();
    firmware::reset_gpio();
}