/*
 * Heavily inspired by:
 * - https://github.com/FrankBau/hello_nucleo
 */

#ifndef __GPIO_H
#define __GPIO_H

// NOLINTBEGIN(modernize-deprecated-headers, hicpp-deprecated-headers)
#include <stdint.h>
// NOLINTEND(modernize-deprecated-headers, hicpp-deprecated-headers)
#include <stm32l432xx.h>

// pin mode
constexpr uint8_t INPUT = 0;
constexpr uint8_t OUTPUT = 1;
constexpr uint8_t INPUT_PULLUP = 4;

// enable clock for a given GPIO port
static inline void enable_port_clock(GPIO_TypeDef* GPIOx)
{
  // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  if (GPIOx == GPIOA)
  {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  }
  else if (GPIOx == GPIOB)
  {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
  }
  else if (GPIOx == GPIOC)
  {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
  }
  // extend for other ports (GPIOD, GPIOE, etc.)
  // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
}

// setup GPIO pin to a specific mode, type, and pull configuration
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
static inline void gpio_setup(GPIO_TypeDef* GPIOx, uint32_t pin, uint8_t setup)
{
  enable_port_clock(GPIOx);

  // MODER: 2 bits per pin: input, output, alternate function, analog
  uint32_t shift2 = pin * 2;
  // NOLINTBEGIN(hicpp-signed-bitwise)
  uint32_t mode = setup & 0x3;
  GPIOx->MODER &= ~(0x3U << shift2);  // clear mode bits
  GPIOx->MODER |= (mode << shift2);   // set new mode

  // PUPDR: 2 bits per pin: no_pull, pull_up, pull_down
  uint32_t pull = (setup >> 2) & 0x3;
  GPIOx->PUPDR &= ~(0x3U << shift2);  // clear pull bits
  GPIOx->PUPDR |= (pull << shift2);   // set new pull
  // NOLINTEND(hicpp-signed-bitwise)
}

// read GPIO pin state
static inline int gpio_read(GPIO_TypeDef* GPIOx, uint32_t pin)
{
  // NOLINTBEGIN(hicpp-signed-bitwise, cppcoreguidelines-narrowing-conversions, bugprone-narrowing-conversions)
  uint32_t shift = pin;
  return (GPIOx->IDR >> shift) & 1U;
  // NOLINTEND(hicpp-signed-bitwise, cppcoreguidelines-narrowing-conversions, bugprone-narrowing-conversions)
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
static inline void gpio_write(GPIO_TypeDef* GPIOx, uint32_t pin, uint8_t level)
{
  // NOLINTBEGIN(hicpp-signed-bitwise)
  uint32_t shift = pin;
  if (level != 0)
  {
    GPIOx->ODR |= (1U << shift);
  }
  else
  {
    GPIOx->ODR &= ~(1U << shift);
  }
  // NOLINTEND(hicpp-signed-bitwise)
}

#endif
