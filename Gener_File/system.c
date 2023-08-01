#include "system.h"
#include "debug.h"
#include "key_scan.h"
#include "led.h"
LED_t led1,led2;
void System_Init()
{
    //定义初始化结构体
    GPIO_InitTypeDef GPIO_InStru={0};
    //开启GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //初始化LED引脚
    //LED1 -> PA1
    //LED2 -> PA2
    GPIO_InStru.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InStru.GPIO_Mode=GPIO_Mode_Out_OD;
    GPIO_InStru.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InStru);


    led1.pin = GPIO_Pin_1;
    led1.port=GPIOA;
    led1.rcc_port=RCC_APB2Periph_GPIOA;

    led2.pin = GPIO_Pin_2;
    led2.port = GPIOA;
    led2.rcc_port = RCC_APB2Periph_GPIOA;

    Led_Init(&led1);
    Led_Init(&led2);

    Key_Init();
}

void System_Loop()
{
    key_scan();
    Key_Action();
}
