// File:          rgbled.h
// Module:        RGBLED
// Description:   Driver for RGB LED
// Resources:     PB0 PB1 PB5

#ifndef __RGBLED_H
#define __RGBLED_H

#define RGBLED_R_CLK    RCC_APB2Periph_GPIOB
#define RGBLED_R_PORT   GPIOB
#define RGBLED_R_PIN    GPIO_Pin_5

#define RGBLED_G_CLK    RCC_APB2Periph_GPIOB
#define RGBLED_G_PORT   GPIOB
#define RGBLED_G_PIN    GPIO_Pin_0

#define RGBLED_B_CLK    RCC_APB2Periph_GPIOB
#define RGBLED_B_PORT   GPIOB
#define RGBLED_B_PIN    GPIO_Pin_1

typedef enum {
  RGBLED_R,
  RGBLED_G,
  RGBLED_B
} RGBLED_Color;

void RGBLED_Init(void);

void RGBLED_Clear(void);
void RGBLED_SetColor(RGBLED_Color color);

#endif
