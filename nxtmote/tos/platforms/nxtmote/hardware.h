/**
 * Adapted for nxtmote.
 * @author Rasmus Pedersen
 */
#ifndef __NXTMOTE_HARDWARE_H__
#define __NXTMOTE_HARDWARE_H__

#include "AT91SAM7S256.h"

#include "AT91SAM7S256_extra.h"
#include "lib_AT91SAM7S256.h"
#include "lib_extra_AT91SAM7S256.h"

extern void AT91F_Spurious_handler(void);
extern void AT91F_Default_IRQ_handler(void);
extern void AT91F_Default_FIQ_handler(void);

inline void __nesc_enable_interrupt() {
  uint32_t statusReg = 0;

  asm volatile (
	       "mrs %0,CPSR\n\t"
	       "bic %0,%1,#0xc0\n\t"
	       "msr CPSR_c, %1"
	       : "=r" (statusReg)
	       : "0" (statusReg)
	       );

  return;
}

inline void __nesc_disable_interrupt() {
  uint32_t statusReg = 0;

  asm volatile (
		"mrs %0,CPSR\n\t"
		"orr %0,%1,#0xc0\n\t"
		"msr CPSR_c,%1\n\t"
		: "=r" (statusReg)
		: "0" (statusReg)
		);

  return;
}

typedef uint32_t __nesc_atomic_t;

inline __nesc_atomic_t __nesc_atomic_start(void) @spontaneous() {
  uint32_t result = 0;
  uint32_t temp = 0;

  asm volatile (
		"mrs %0,CPSR\n\t"
		"orr %1,%2,%4\n\t"
		"msr CPSR_cf,%3"
		: "=r" (result) , "=r" (temp)
		: "0" (result) , "1" (temp) , "i" (ARM_CPSR_INT_MASK)
		);
  return result;
}

inline void __nesc_atomic_end(__nesc_atomic_t oldState) @spontaneous() {
  uint32_t  statusReg = 0;

  oldState &= ARM_CPSR_INT_MASK;

  asm volatile (
		"mrs %0,CPSR\n\t"
		"bic %0, %1, %2\n\t"
		"orr %0, %1, %3\n\t"
		"msr CPSR_c, %1"
		: "=r" (statusReg)
		: "0" (statusReg),"i" (ARM_CPSR_INT_MASK), "r" (oldState)
		);

  return;
}
inline void __nesc_atomic_sleep() {
  __nesc_enable_interrupt();
}

// Add offset to channel id (first channel id is zero) to get timer peripheral id
#define TIMER_PID(chnl_id) chnl_id + AT91C_ID_TC0

// With MCK at 48054857 Hz and a timer incrementing at MCK/2
// 1 ms is approx. 24028 ticks
#define TICKSONEMSCLK2 (24028)
//#define TICKTOMSCLK2(ticks)

// priorities which are used in HplAt91InterruptM
const uint8_t TOSH_IRP_TABLE[] = {
  0xFF, // AT91C_ID_FIQ         ID  0, Advanced Interrupt Controller (FIQ)
  0xFF, // AT91C_ID_SYS         ID  1, System Peripheral
  0xFF, // AT91C_ID_PIOA        ID  2, Parallel IO Controller
  0xFF, // AT91C_ID_3_Reserved  ID  3, Reserved
  0xFF, // AT91C_ID_ADC         ID  4, Analog-to-Digital Converter
  0xFF, // AT91C_ID_SPI         ID  5, Serial Peripheral Interface
  0xFF, // AT91C_ID_US0         ID  6, USART 0
  0xFF, // AT91C_ID_US1         ID  7, USART 1
  0xFF, // AT91C_ID_SSC         ID  8, Serial Synchronous Controller
  0xFF, // AT91C_ID_TWI         ID  9, Two-Wire Interface
  0xFF, // AT91C_ID_PWMC        ID 10, PWM Controller
  0xFF, // AT91C_ID_UDP         ID 11, USB Device Port
  0x04, // AT91C_ID_TC0         ID 12, Timer Counter 0
  0xFF, // AT91C_ID_TC1         ID 13, Timer Counter 1
  0xFF, // AT91C_ID_TC2         ID 14, Timer Counter 2
  0xFF, // AT91C_ID_15_Reserved ID 15, Reserved
  0xFF, // AT91C_ID_16_Reserved ID 16, Reserved
  0xFF, // AT91C_ID_17_Reserved ID 17, Reserved
  0xFF, // AT91C_ID_18_Reserved ID 18, Reserved
  0xFF, // AT91C_ID_19_Reserved ID 19, Reserved
  0xFF, // AT91C_ID_20_Reserved ID 20, Reserved
  0xFF, // AT91C_ID_21_Reserved ID 21, Reserved
  0xFF, // AT91C_ID_22_Reserved ID 22, Reserved
  0xFF, // AT91C_ID_23_Reserved ID 23, Reserved
  0xFF, // AT91C_ID_24_Reserved ID 24, Reserved
  0xFF, // AT91C_ID_25_Reserved ID 25, Reserved
  0xFF, // AT91C_ID_26_Reserved ID 26, Reserved
  0xFF, // AT91C_ID_27_Reserved ID 27, Reserved
  0xFF, // AT91C_ID_28_Reserved ID 28, Reserved
  0xFF, // AT91C_ID_29_Reserved ID 29, Reserved
  0xFF, // AT91C_ID_IRQ0        ID 30, Advanced Interrupt Controller (IRQ0)
  0xFF, // AT91C_ID_IRQ1        ID 31, Advanced Interrupt Controller (IRQ1)
  0xFF, // AT91C_ALL_INT        ID 0xC0007FF7 ALL VALID INTERRUPTS
};

// Lookup table for some peripheral ids
AT91_REG PID_ADR_TABLE[] = {
  0x00000000,     // AT91C_ID_FIQ         ID  0, Advanced Interrupt Controller (FIQ)
  0x00000000,     // AT91C_ID_SYS         ID  1, System Peripheral
  0x00000000,     // AT91C_ID_PIOA        ID  2, Parallel IO Controller
  0x00000000,     // AT91C_ID_3_Reserved  ID  3, Reserved
  0x00000000,     // AT91C_ID_ADC         ID  4, Analog-to-Digital Converter
  0x00000000,     // AT91C_ID_SPI         ID  5, Serial Peripheral Interface
  0x00000000,     // AT91C_ID_US0         ID  6, USART 0
  0x00000000,     // AT91C_ID_US1         ID  7, USART 1
  0x00000000,     // AT91C_ID_SSC         ID  8, Serial Synchronous Controller
  0x00000000,     // AT91C_ID_TWI         ID  9, Two-Wire Interface
  0x00000000,     // AT91C_ID_PWMC        ID 10, PWM Controller
  0x00000000,     // AT91C_ID_UDP         ID 11, USB Device Port
  AT91C_BASE_TC0, // AT91C_ID_TC0         ID 12, Timer Counter 0
  AT91C_BASE_TC1, // AT91C_ID_TC1         ID 13, Timer Counter 1
  AT91C_BASE_TC2, // AT91C_ID_TC2         ID 14, Timer Counter 2
  0x00000000,     // AT91C_ID_15_Reserved ID 15, Reserved
  0x00000000,     // AT91C_ID_16_Reserved ID 16, Reserved
  0x00000000,     // AT91C_ID_17_Reserved ID 17, Reserved
  0x00000000,     // AT91C_ID_18_Reserved ID 18, Reserved
  0x00000000,     // AT91C_ID_19_Reserved ID 19, Reserved
  0x00000000,     // AT91C_ID_20_Reserved ID 20, Reserved
  0x00000000,     // AT91C_ID_21_Reserved ID 21, Reserved
  0x00000000,     // AT91C_ID_22_Reserved ID 22, Reserved
  0x00000000,     // AT91C_ID_23_Reserved ID 23, Reserved
  0x00000000,     // AT91C_ID_24_Reserved ID 24, Reserved
  0x00000000,     // AT91C_ID_25_Reserved ID 25, Reserved
  0x00000000,     // AT91C_ID_26_Reserved ID 26, Reserved
  0x00000000,     // AT91C_ID_27_Reserved ID 27, Reserved
  0x00000000,     // AT91C_ID_28_Reserved ID 28, Reserved
  0x00000000,     // AT91C_ID_29_Reserved ID 29, Reserved
  0x00000000,     // AT91C_ID_IRQ0        ID 30, Advanced Interrupt Controller (IRQ0)
  0x00000000,     // AT91C_ID_IRQ1        ID 31, Advanced Interrupt Controller (IRQ1)
  0x00000000,     // AT91C_ALL_INT        ID 0xC0007FF7 ALL VALID INTERRUPTS
};

//Clock selection constants
//enum {
//  TC_CLKS         = 0x7,
//  TC_CLKS_MCK2    = 0x0,
//  TC_CLKS_MCK8    = 0x1,
//  TC_CLKS_MCK32   = 0x2,
//  TC_CLKS_MCK128  = 0x3,
//  TC_CLKS_MCK1024 = 0x4
//};

  // Little function to toggle a pin
#define togglepin(toggle)	unsigned long* PIOPER_;\
							unsigned long* PIOOER_;\
							unsigned long* PIOCODR_;\
							unsigned long* PIOSODR_;\
							unsigned long LEDVAL_;\
							/* GPIO register addresses */\
							PIOPER_ = (unsigned long*)0xFFFFF400;\
							PIOOER_ =  (unsigned long*)0xFFFFF410;\
							PIOCODR_ =  (unsigned long*)0xFFFFF434;\
							PIOSODR_ = (unsigned long*)0xFFFFF430; /* (PIOA) Set Output Data Register */\
							/* Register use */\
							LEDVAL_ =  ((unsigned long) 1 << 18);/*(unsigned long)0x00000008;*/\
							*PIOPER_ = LEDVAL_;\
							*PIOOER_ = LEDVAL_;\
							if(toggle == 0)\
							  *PIOCODR_ = LEDVAL_;  /* port 1 pin 6 at 0.0 v (enable this line OR the next)*/\
							else\
							  *PIOSODR_ = LEDVAL_;  /* port 1 pin 6 (blue) at 3.25-3.27 v (GND is on pin 2 (black)) */\
							//while(1); /* stop here */

#endif //__NXTMOTE_HARDWARE_H__