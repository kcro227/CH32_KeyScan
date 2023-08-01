/*
 * led.c
 *
 *  Created on: 2023年7月27日
 *      Author: Administrator
 */
#include "led.h"

/**
 * LED初始化函数
 * 参数：LED结构体指针
 */
void Led_Init(LED_t *LEDx) {

    RCC_APB2PeriphClockCmd(LEDx->rcc_port, ENABLE);

    GPIO_InitTypeDef LED_GPIO_t;
    LED_GPIO_t.GPIO_Mode = GPIO_Mode_Out_OD;
    LED_GPIO_t.GPIO_Pin = LEDx->pin;
    LED_GPIO_t.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(LEDx->port, &LED_GPIO_t);

}
/**
 * LED 开启函数，传入LED_t结构体指针
 */

void LED_On(LED_t *LEDx)
{
    GPIO_ResetBits(LEDx->port, LEDx->pin);
}
/**
 * LED关闭函数
 */
void LED_Off(LED_t *LEDx)
{
    GPIO_SetBits(LEDx->port, LEDx->pin);
}
/**
 * LED翻转函数
 */
void LED_Toggle(LED_t *LEDx)
{
    GPIO_WriteBit(LEDx->port, LEDx->pin, !GPIO_ReadOutputDataBit(LEDx->port, LEDx->pin));
}
