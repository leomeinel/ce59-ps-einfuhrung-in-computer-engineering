<!-- omit in toc -->
# NUCLEO-L432KC Aufgabe

|                 |                                                   |
| --------------- | ------------------------------------------------- |
| **Verfasser**   | Leopold Johannes Meinel                           |
| **Datum**       | 2025-12-31                                        |
| **Studiengang** | Computer Engineering (B)                          |
| **Fachbereich** | Ingenieurwissenschaften – Energie und Information |
| **Einrichtung** | Hochschule für Technik und Wirtschaft Berlin      |
| **Betreuer**    | Prof. Bauernöppel                                 |

<div style="page-break-after: always;"></div>

# Inhaltsverzeichnis

1. [Inhaltsverzeichnis](#inhaltsverzeichnis)
2. [Protokoll](#protokoll)
   1. [Aufgabe 1](#aufgabe-1)
      1. [Code diff](#code-diff)
      2. [Fragen](#fragen)
   2. [Aufgabe 2](#aufgabe-2)
      1. [Code](#code)
      2. [Beobachtungen](#beobachtungen)
      3. [Anmerkungen](#anmerkungen)

# Protokoll

## Aufgabe 1

### Code diff

```diff
     if (c == 't' || button == 0) {
       gpio_write(GPIOB, 3, 1);
-      delay_ms(3000);
+      delay_ms(5000);
       gpio_write(GPIOB, 3, 0);
     }
```

### Fragen

> Wann genau beginnt das Leuchten, wenn Sie den Taster drücken oder wenn Sie ihn loslassen?

- Das Leuchten beginnt, nachdem der button gedrückt wurde.

> Wann genau endet das Leuchten, 5 Sekunden nach dem Drücken oder nach dem Loslassen?

- Das Leuchten endet 5 Sekunden nach dem Drücken des buttons, wenn dieser zu diesem Zeitpunkt nicht gedrückt gehalten wird.
- Falls der button zu diesem Zeitpunkt gedrückt gehalten wird, leuchtet es mindestens bis man den button loslässt bzw. maximal bis 5 Sekunden nach dem Loslassen. Dies würde ich als undefined behavior definieren.

<div style="page-break-after: always;"></div>

## Aufgabe 2

### Code

```cpp
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
```

<div style="page-break-after: always;"></div>

### Beobachtungen

| Geändertes Verhalten                                             | Gleich gebliebenes Verhalten                                                             |
| ---------------------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| Das Leuchten beginnt, nachdem der button losgelassen wurde.      | Die LED `LD3` und die an pin `D13` verbundene LED leuchten nach dem Drücken des buttons. |
| Das Leuchten endet `40 * 100ms` nach dem Loslassen des buttons.  | Bei wiederholtem drücken des buttons wird das Leuchtfenster jedes Mal zurückgesetzt.     |
| Bei gedrückt halten des buttons gibt es kein undefined behavior. |                                                                                          |

### Anmerkungen

Ich habe an dem code relativ viel verändert, damit dieser gut lesbar und dokumentiert ist.

Außerdem habe ich die verlangten Screenshots anstattdessen direkt in markdown als codeblock eingefügt bzw. als diff.
