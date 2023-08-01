/*
 * key_scan.c
 *
 *  Created on: 2023年7月4日
 *      Author: Administrator
 */
#include "key_scan.h"
#include "led.h"
extern LED_t led1, led2;
FSM_value_t FSM_value = { FSM_KEY_Up, 0, 0 };
;
KEY_t KEY_whitch;
void Key_Init() {
    GPIO_InitTypeDef KeyInitStructure;
    TIM_TimeBaseInitTypeDef TIM6_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    TIM6_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM6_InitStructure.TIM_Period = 1000;
    TIM6_InitStructure.TIM_Prescaler = (SystemCoreClock / 1000000 - 1);
    TIM_TimeBaseInit(TIM6, &TIM6_InitStructure);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM6, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStructure);

    KeyInitStructure.GPIO_Mode = GPIO_Mode_IPU;
    KeyInitStructure.GPIO_Pin = GPIO_Pin_0;
    KeyInitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &KeyInitStructure);

}

void key_scan() {
    static uint16_t press_count = 0,run_count = 0;

    if (FSM_value.cnt >= 5) {
        FSM_value.cnt = 0;

        switch (FSM_value.state) {
        //按键弹起状态
        case FSM_KEY_Up: {
            switch (KEY_USER_READ()) {
            //按键弹起
            case KEY_UP: {

                break;
            }
                //按键按下
            case KEY_DOWN: {
                FSM_value.state = FSM_KEY_DownShake;

                break;
            }
            }
            break;
        }

            //按键按下抖动状态
        case FSM_KEY_DownShake: {
            switch (KEY_USER_READ()) {
            //按键弹起
            case KEY_UP: { //弹起表明为抖动，恢复原状态
                FSM_value.state = FSM_KEY_Up;
                break;
            }
                //按键按下
            case KEY_DOWN: { //确实按下，则为按键按下状态
                FSM_value.state = FSM_KEY_Down;
                break;
            }
            }
            break;
        }

            //按键按下状态
        case FSM_KEY_Down: {
            switch(KEY_USER_READ()) {
                case KEY_UP:{//如果按键没被按下，则切换为弹起抖动状态
                    FSM_value.state = FSM_KEY_UpShake;
                }
                case KEY_DOWN: {//确认按下
                    switch(FSM_value.auxiliary_flag)
                    {
                        case 0:{
                            if(++press_count == 100)
                            {
                                press_count = 0;
                                KEY_whitch.id = KEY_USER;
                                KEY_whitch.action = KEY_PRESS;
                                FSM_value.auxiliary_flag = 0;

                                printf("key_press\n");
                            }
                            break;
                        }
                        case 1:{
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }

            //按键弹起抖动状态
        case FSM_KEY_UpShake: {
            switch (KEY_USER_READ()) {
        case KEY_UP:{
            if(run_count < 80)
            {
            if (FSM_value.auxiliary_flag) {
                KEY_whitch.id = KEY_USER;
                KEY_whitch.action = KEY_DOUBLE;
                FSM_value.auxiliary_flag = 0;
                printf("key_double\n");
                }
            else {
                FSM_value.state = FSM_KEY_Up;
                FSM_value.auxiliary_flag = 1;
                KEY_whitch.id = KEY_USER;
                KEY_whitch.action = KEY_CHICK;
                printf("key_chick\n");
                }
            }

            break;
        }
        case KEY_DOWN: {
            FSM_value.state = FSM_KEY_Down;
            break;
        }
            }
            break;
        }
        }

        }


}

void Key_Action()
{
    if(KEY_whitch.id)
        printf("key id:%d,key ac:%d\n",KEY_whitch.id,KEY_whitch.action);
    switch(KEY_whitch.id)
    {
    case KEY_USER:{
        switch(KEY_whitch.action){
        case KEY_CHICK:{
            LED_Toggle(&led2);
            KEY_whitch.id = KEY_NULL;
            KEY_whitch.action = KEY_RELESS;
            break;
        }
        case KEY_DOUBLE:{
            LED_Off(&led2);
            KEY_whitch.id = KEY_NULL;
            KEY_whitch.action = KEY_RELESS;
            break;
        }
        case KEY_PRESS:{
            LED_On(&led2);
            KEY_whitch.id = KEY_NULL;
            KEY_whitch.action = KEY_RELESS;
            break;
        }
        }
        break;
    }
    default :{
        break;
    }
    }
}
void TIM6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM6_IRQHandler(void) {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET) {
        static volatile uint16_t i = 0;
        if (++FSM_value.cnt > 254)
            FSM_value.cnt = 0;
        if (++i == 500) {
            LED_Toggle(&led1);
            i = 0;
        }
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

