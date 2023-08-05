/*
 * key_scan.c
 *
 *  Created on: 2023年7月4日
 *      Author: Administrator
 */
#include "key_scan.h"

//函数声明
static uint8_t KEY1_READ(void);
static uint8_t KEY2_READ(void);




//变量声明
KEY_OBJECT Key_Obj[KEY_NUM] = {//添加按键需初始化按键结构体
        { 0, 0, 0, 0, 0, 1, KEY1_READ, _KEY_STATUS_RELEASE, _KEY_EVEN_NULL },
        { 0, 0, 0, 0, 0, 1,KEY2_READ,_KEY_STATUS_RELEASE,_KEY_EVEN_NULL},
};
extern LED_t led1,led2;
uint8_t FSM_TIM_Count = 0;
/*
 * 在此添加按键GPIO初始化函数*/
void KEY_Init(void) {//GPIO TIMER6 NVIC 初始化
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
    KeyInitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;
    KeyInitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &KeyInitStructure);

}
/*在此添加按键读取函数
 *
 * */
static uint8_t KEY1_READ(void) {
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}
static uint8_t KEY2_READ(void) {
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
}
/*

以下函数无需更改
*/
static void _KEY_Statue_Release_Handle(uint8_t whitch_key) {//按键释放状态
    if (Key_Obj[whitch_key].Read_Key() == Key_Obj[whitch_key].Key_Down_Level) {
        Key_Obj[whitch_key].statue = _KEY_STATUS_DOWNSHAKE;//若检测到按键按下，则进入按下抖动状态

    }
    if (Key_Obj[whitch_key].Press_Interval >= _KEY_INTERVAL_TIME) {//判断按压间隔，若按压间隔大于阈值，说明按键按下过，并且非双击动作，判断为按键单击
        printf("key%d click\n",whitch_key+1);                                         //并将标志清空
        Key_Obj[whitch_key].run_flag = 0;
        Key_Obj[whitch_key].Press_Assic = 0;
        Key_Obj[whitch_key].Press_Again = 0;
        Key_Obj[whitch_key].even = _KEY_EVEN_CLICK;
        Key_Obj[whitch_key].Press_Interval = 0;

    }

}

static void _KEY_Statue_ReleaseShake_Handle(uint8_t whitch_key) {
//按键弹起抖动状态，当按键确认弹起是时，将进入按键弹起状态
    if (Key_Obj[whitch_key].Read_Key() != Key_Obj[whitch_key].Key_Down_Level) //按键弹起
            {
        Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASE;
        if (Key_Obj[whitch_key].Press_Again) {//如果按键按下过，并且按压间隔小于阈值，则判断为双击，并清空标志
            if (Key_Obj[whitch_key].Press_Interval <= _KEY_INTERVAL_TIME) {
                printf("key%d double click\n",whitch_key+1);
                Key_Obj[whitch_key].run_flag = 0;
                Key_Obj[whitch_key].Press_Interval = 0;

                Key_Obj[whitch_key].Press_Again = 0;
                Key_Obj[whitch_key].even = _KEY_EVEN_DOUBLECLICK;
                Key_Obj[whitch_key].Press_Assic = 0;

            }

        } else {//否则为第一次按压，让按键间隔开始计数

            Key_Obj[whitch_key].Press_Assic = 1;
            Key_Obj[whitch_key].Press_Again = 1;

        }
    } else {//如果并没有弹起，则为抖动，恢复上一个状态
        Key_Obj[whitch_key].statue = _KEY_STATUS_DOWN;
    }
}

static void _KEY_Statue_Down_Handle(uint8_t whitch_key) {
    if (Key_Obj[whitch_key].Press_Again) {//按键按压状态，如果按压过，则将进入弹起抖动状态
        if (Key_Obj[whitch_key].Read_Key()
                != Key_Obj[whitch_key].Key_Down_Level) {
            Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASESHAKE;

        }
    } else {//否则判断按压时间，如果大于阈值，则判断为长按，清空标志
        if (++Key_Obj[whitch_key].Press_Long >= _KEY_LONG_TIME) {
            Key_Obj[whitch_key].even = _KEY_EVEN_LONGPRESS;
            Key_Obj[whitch_key].Press_Long = 0;
            Key_Obj[whitch_key].run_flag = 0;
            printf("key%d long press \n",whitch_key+1);
        } else if (Key_Obj[whitch_key].Read_Key()//如果按键在判定为长按之前就弹起，则进入弹起抖动双胎
                != Key_Obj[whitch_key].Key_Down_Level) {
            Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASESHAKE;
        }
    }
}

static void _KEY_Statue_DownShake_Handle(uint8_t whitch_key) {//按下抖动状态
    if (Key_Obj[whitch_key].Read_Key() == Key_Obj[whitch_key].Key_Down_Level) {
        Key_Obj[whitch_key].statue = _KEY_STATUS_DOWN;
    } else {
        Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASE;
    }
}

static void _KEY_Press_Reset_Handle(uint8_t whitch_key) {//按键复位，等待按键弹起。在判断按压事件后，run_flag清零，将进入此状态。
    Key_Obj[whitch_key].run_flag = 1;
    Key_Obj[whitch_key].even = _KEY_EVEN_NULL;
    Key_Obj[whitch_key].Press_Again = 0;
    Key_Obj[whitch_key].Press_Interval = 0;
    Key_Obj[whitch_key].Press_Long = 0;
    Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASE;
}

void KEY_StatuMachine() {

    if (FSM_TIM_Count >= 5) {
        FSM_TIM_Count = 0;
        for (uint8_t i = 0; i < KEY_NUM; i++) {
            if (Key_Obj[i].run_flag) {
                if (Key_Obj[i].Press_Assic)
                    ++Key_Obj[i].Press_Interval;
                else {
                    Key_Obj[i].Press_Interval = 0;
                }

                switch (Key_Obj[i].statue) {
                case _KEY_STATUS_RELEASE: {
                    _KEY_Statue_Release_Handle(i);
                    break;
                }
                case _KEY_STATUS_DOWNSHAKE: {
                    _KEY_Statue_DownShake_Handle(i);
                    break;
                }
                case _KEY_STATUS_DOWN: {
                    _KEY_Statue_Down_Handle(i);
                    break;
                }
                case _KEY_STATUS_RELEASESHAKE: {
                    _KEY_Statue_ReleaseShake_Handle(i);
                    break;
                }
                }
            } else if (Key_Obj[i].Read_Key() != Key_Obj[i].Key_Down_Level) {
                _KEY_Press_Reset_Handle(i);

            }
        }
    }
}


void TIM6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM6_IRQHandler(void) {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET) {

        if (++FSM_TIM_Count > 10)
            FSM_TIM_Count = 0;
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }

}

