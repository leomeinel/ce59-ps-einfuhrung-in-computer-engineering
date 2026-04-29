/*
 * Heavily inspired by:
 * - https://github.com/FrankBau/hello_nucleo
 */

#ifndef DELAY_H
#define DELAY_H

// NOLINTBEGIN(modernize-deprecated-headers, hicpp-deprecated-headers)
#include <stdint.h>
// NOLINTEND(modernize-deprecated-headers, hicpp-deprecated-headers)

// microsecond delay, based on cortex-m4 instruction timing, assume default 4
// MHz SYSCLK
static inline void delay_us(uint32_t delay)
{
  // NOLINTBEGIN(hicpp-no-assembler)
  __asm volatile(
      "mov r0, %[count]\n"  // Load the delay count into register r0
      "1:\n"                // Loop label
      "subs r0, r0, #1\n"   // Subtract 1 from r0   (1 CPU cycle)
      "nop\n"               // nop = 1 CPU cycle
      "bne 1b\n"            // Branch to loop label if r0 is not zero (2 CPU cycles, except
                            // for Cortex-M0)
      :
      : [count] "r"(delay)  // Input: delay_count is the number of iterations
      : "r0"                // Clobbered register
  );
  // NOLINTEND(hicpp-no-assembler)
}

// millisecond delay
static inline void delay_ms(uint32_t delay)
{
#pragma unroll 2
  for (uint32_t i = 0; i < delay; ++i)
  {
    constexpr uint16_t DELAY_ONE_SECOND = 1000;
    delay_us(DELAY_ONE_SECOND);
  }
}

#endif
