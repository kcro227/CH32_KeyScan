/*
 * led.c
 *
 *  Created on: 2023��7��27��
 *      Author: Administrator
 */
#include "led.h"

/**
 * LED��ʼ������
 * ������LED�ṹ��ָ��
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
 * LED ��������������LED_t�ṹ��ָ��
 */

void LED_On(LED_t *LEDx)
{
    GPIO_ResetBits(LEDx->port, LEDx->pin);
}
/**
 * LED�رպ���
 */
void LED_Off(LED_t *LEDx)
{
    GPIO_SetBits(LEDx->port, LEDx->pin);
}
/**
 * LED��ת����
 */
void LED_Toggle(LED_t *LEDx)
{
    GPIO_WriteBit(LEDx->port, LEDx->pin, !GPIO_ReadOutputDataBit(LEDx->port, LEDx->pin));
}
