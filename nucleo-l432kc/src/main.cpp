/*
 * Heavily inspired by:
 * - https://github.com/FrankBau/hello_nucleo
 */

// NOLINTBEGIN(modernize-deprecated-headers, hicpp-deprecated-headers)
#include <stdint.h>
// NOLINTEND(modernize-deprecated-headers, hicpp-deprecated-headers)

#include "delay.h"
#include "gpio.h"
#include "uart.h"

/**
 * @brief Data pin that is connected to the button
 *
 */
constexpr uint8_t PIN_BUTTON = 0;

/**
 * @brief Data pin that is connected to the led
 *
 */
constexpr uint8_t PIN_LED = 3;

/**
 * @brief Level that indicates that something is off
 *
 */
constexpr uint8_t LEVEL_OFF = 0;

/**
 * @brief Level that indicates that something is on
 *
 */
constexpr uint8_t LEVEL_ON = 1;

namespace
{
  /**
   * @brief Initialize pins
   *
   */
  void init_pins()
  {
    // Setup pins
    // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    gpio_setup(GPIOB, PIN_LED, OUTPUT);
    gpio_setup(GPIOB, PIN_BUTTON, INPUT_PULLUP);

    // Test PIN_LED and set initial level
    constexpr uint32_t DURATION_TEST = 1000;
    gpio_write(GPIOB, PIN_LED, LEVEL_ON);
    delay_ms(DURATION_TEST);
    gpio_write(GPIOB, PIN_LED, LEVEL_OFF);
    // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  }

  /**
   * @brief Init uart console and output help text
   *
   */
  void print_help_uart()
  {
    uart_init();

    constexpr auto HELP_UART =
        "Welcome!\n"
        "Press 't' or toggle the button in the circuit to turn on the light.\n";
    uart_puts(HELP_UART);
  }
}  // namespace

int main()
{
  // Initialize board and print help
  init_pins();
  print_help_uart();

  // Initialize counter for main loop
  uint8_t counter = 0;

  // main loop
#pragma unroll
  while (true)
  {
    // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    uint8_t button_state = gpio_read(GPIOB, PIN_BUTTON);
    char uart_input_char = uart_getc();
    constexpr auto UART_TOGGLE_CHAR = 't';

    if (button_state == LEVEL_OFF || uart_input_char == UART_TOGGLE_CHAR)
    {
      // If toggled via button or uart, set counter to 40
      constexpr uint8_t COUNTER_INIT = 40;
      counter = COUNTER_INIT;
    }
    else if (counter > 0)
    {
      // If counter is above 0, toggle PIN_LED on and decrement counter
      gpio_write(GPIOB, PIN_LED, LEVEL_ON);
      counter--;
    }
    else
    {
      // Otherwise, toggle PIN_LED off
      gpio_write(GPIOB, PIN_LED, LEVEL_OFF);
    }

    // Delay the loop
    constexpr uint32_t DELAY_LOOP = 100;
    delay_ms(DELAY_LOOP);
    // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  }
}
