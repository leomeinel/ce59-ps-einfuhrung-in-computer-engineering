/*
 * Heavily inspired by:
 * - https://github.com/FrankBau/hello_nucleo
 */

#ifndef __UART_H
#define __UART_H

#include <stm32l432xx.h>

// setup of UART (USART2) peripheral block in the microcontroller

// see STM32L43xxx reference manual (RM0394 Rev 4) for USART2 registers, bits,
// and function see STM32L432KC data sheet (DS11451 Rev 4) for alternate
// function (AF) and pin assignment see Nucleo-32 board user guide (UM1956 Rev
// 5) for NUCLEO-L432KC board schematics (wiring)

// USB VCP virtual com port connection
// PA2    ------> USART2_TX     pin alternate function: AF7 (see data sheet)
// PA15   ------> USART2_RX     pin alternate function: AF3 (see data sheet)

// USART2 initialization at 115200 baud 8N1
static inline void uart_init()
{
  // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr, hicpp-signed-bitwise)
  // setup UART pins:
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  // enable clock for peripheral component
  (void)RCC->AHB2ENR;                   // ensure that the last write command finished and the
                                        // clock is on

  GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE2_Msk) | (2 << GPIO_MODER_MODE2_Pos);  // set AF mode
  constexpr uint8_t PIN_AF7 = 7;
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~GPIO_AFRL_AFSEL2_Msk) | (PIN_AF7 << GPIO_AFRL_AFSEL2_Pos);  // set pin AF7

  GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE10_Msk) | (2 << GPIO_MODER_MODE10_Pos);  // set AF mode
  constexpr uint8_t PIN_AF3 = 3;
  GPIOA->AFR[1] = (GPIOA->AFR[1] & ~GPIO_AFRH_AFSEL15_Msk) | (PIN_AF3 << GPIO_AFRH_AFSEL15_Pos);  // set pin AF3

  // setup USART2

  RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;  // enable clock for peripheral component
  (void)RCC->APB1ENR1;                     // ensure that the last instruction finished and the clock is now on

  constexpr int SYSCLK = 4000000;
  constexpr int BAUD_RATE = 115200;
  USART2->BRR = SYSCLK / BAUD_RATE;                          // set baud rate = 115200 baud, assuming SYSCLK = 4 MHz
  USART2->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;  // enable UART, RX, TX
  // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr, hicpp-signed-bitwise)
}

// blocking version of putc: waits until character is sent
static inline void uart_putc(char character)
{
// NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
#pragma unroll
  while ((USART2->ISR & USART_ISR_TXE) == 0U)
  {
  }  // wait until transmit data register is empty
  USART2->TDR = character;  // write character to data register
  // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
}

// blocking version of puts: waits until all characters are sent
static inline void uart_puts(const char* string_)
{
#pragma unroll
  while (*string_ != 0U)
  {
    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    uart_putc(*string_++);
    // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }
}

// non-blocking version of getc: returns -1 if no character is available
static inline int uart_getc()
{
  // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  USART2->ICR = USART_ICR_ORECF;  // clear overrun error flag (set when chars were missed)
  if (USART2->ISR & USART_ISR_RXNE)
  {                                      // RXNE flag set?
    char character = (char)USART2->RDR;  // reading RDR automagically clears the RXNE flag
    return character;
  }
  return -1;  // no character received
  // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
}

#endif
