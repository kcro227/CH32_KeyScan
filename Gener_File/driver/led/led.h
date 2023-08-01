/*
 * led.h
 *
 *  Created on: 2023Äê7ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef LED_LED_H_
#define LED_LED_H_

#include "debug.h"

#define LED1_PORT GPIOA
#define LED1_PIN GPIO_Pin_1
#define LED1_PORT GPIOA
#define LED1_PIN GPIO_Pin_1

typedef struct LED_t
{

    GPIO_TypeDef *port;
    uint32_t rcc_port;
    uint16_t pin;
}LED_t;

void Led_Init(LED_t *LEDx);
void LED_On(LED_t *LEDx);
void LED_Off(LED_t *LEDx);
void LED_Toggle(LED_t *LEDx);


#endif /* LED_LED_H_ */


