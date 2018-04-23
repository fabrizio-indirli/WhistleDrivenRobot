
/*
 * Extremely simplified code to allow accessing peripheral registers from C/C++
 * In a real application one would use the register definition in the CMSIS.
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

/*
 * Reset and Clock Control. This peripheral is used to turn on the other peripherals
 */

typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t PLLCFGR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t AHB1RSTR;
  volatile uint32_t AHB2RSTR;
  volatile uint32_t AHB3RSTR;
  uint32_t          RESERVED0;
  volatile uint32_t APB1RSTR;
  volatile uint32_t APB2RSTR;
  uint32_t          RESERVED1[2];
  volatile uint32_t AHB1ENR;
  volatile uint32_t AHB2ENR;
  volatile uint32_t AHB3ENR;
  uint32_t          RESERVED2;
  volatile uint32_t APB1ENR;
  volatile uint32_t APB2ENR;
} RCC_TypeDef;

#define RCC ((RCC_TypeDef *)0x40023800)
#define RCC_AHB1ENR_GPIOAEN  (0x00000001)
#define RCC_AHB1ENR_GPIODEN  (0x00000008)
#define RCC_APB1ENR_USART2EN (0x00020000)

/*
 * GPIOD. This peripheral controls port D of the GPIOs
 */

typedef struct
{
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *)0x40020000)
#define GPIOD ((GPIO_TypeDef *)0x40020C00)

/*
 * USART2. This peripheral is a serial port
 */

typedef struct
{
  volatile uint16_t SR;
  uint16_t          RESERVED0;
  volatile uint16_t DR;
  uint16_t          RESERVED1;
  volatile uint16_t BRR;
  uint16_t          RESERVED2;
  volatile uint16_t CR1;
  uint16_t          RESERVED3;
  volatile uint16_t CR2;
  uint16_t          RESERVED4;
  volatile uint16_t CR3;
  uint16_t          RESERVED5;
  volatile uint16_t GTPR;
  uint16_t          RESERVED6;
} USART_TypeDef;

#define USART2 ((USART_TypeDef *)0x40004400)

#define  USART_SR_RXNE    (0x0020)
#define  USART_SR_TXE     (0x0080)

#define  USART_CR1_RE     (0x0004)
#define  USART_CR1_TE     (0x0008)
#define  USART_CR1_RXNEIE (0x0020)
#define  USART_CR1_UE     (0x2000)

/*
 * NVIC. Nested vectored interrupt controller.
 * This peripheral controls all interrupts in the microcontroller
 */

typedef struct
{
   volatile uint32_t ISER[8];
} NVIC_Type;

#define NVIC ((NVIC_Type*)0xE000E100)

#endif //REGISTERS_H
