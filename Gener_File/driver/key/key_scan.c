/*
 * key_scan.c
 *
 *  Created on: 2023��7��4��
 *      Author: Administrator
 */
#include "key_scan.h"

//��������
static uint8_t KEY1_READ(void);
static uint8_t KEY2_READ(void);




//��������
KEY_OBJECT Key_Obj[KEY_NUM] = {//��Ӱ������ʼ�������ṹ��
        { 0, 0, 0, 0, 0, 1, KEY1_READ, _KEY_STATUS_RELEASE, _KEY_EVEN_NULL },
        { 0, 0, 0, 0, 0, 1,KEY2_READ,_KEY_STATUS_RELEASE,_KEY_EVEN_NULL},
};
extern LED_t led1,led2;
uint8_t FSM_TIM_Count = 0;
/*
 * �ڴ���Ӱ���GPIO��ʼ������*/
void KEY_Init(void) {//GPIO TIMER6 NVIC ��ʼ��
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
/*�ڴ���Ӱ�����ȡ����
 *
 * */
static uint8_t KEY1_READ(void) {
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}
static uint8_t KEY2_READ(void) {
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
}
/*

���º����������
*/
static void _KEY_Statue_Release_Handle(uint8_t whitch_key) {//�����ͷ�״̬
    if (Key_Obj[whitch_key].Read_Key() == Key_Obj[whitch_key].Key_Down_Level) {
        Key_Obj[whitch_key].statue = _KEY_STATUS_DOWNSHAKE;//����⵽�������£�����밴�¶���״̬

    }
    if (Key_Obj[whitch_key].Press_Interval >= _KEY_INTERVAL_TIME) {//�жϰ�ѹ���������ѹ���������ֵ��˵���������¹������ҷ�˫���������ж�Ϊ��������
        printf("key%d click\n",whitch_key+1);                                         //������־���
        Key_Obj[whitch_key].run_flag = 0;
        Key_Obj[whitch_key].Press_Assic = 0;
        Key_Obj[whitch_key].Press_Again = 0;
        Key_Obj[whitch_key].even = _KEY_EVEN_CLICK;
        Key_Obj[whitch_key].Press_Interval = 0;

    }

}

static void _KEY_Statue_ReleaseShake_Handle(uint8_t whitch_key) {
//�������𶶶�״̬��������ȷ�ϵ�����ʱ�������밴������״̬
    if (Key_Obj[whitch_key].Read_Key() != Key_Obj[whitch_key].Key_Down_Level) //��������
            {
        Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASE;
        if (Key_Obj[whitch_key].Press_Again) {//����������¹������Ұ�ѹ���С����ֵ�����ж�Ϊ˫��������ձ�־
            if (Key_Obj[whitch_key].Press_Interval <= _KEY_INTERVAL_TIME) {
                printf("key%d double click\n",whitch_key+1);
                Key_Obj[whitch_key].run_flag = 0;
                Key_Obj[whitch_key].Press_Interval = 0;

                Key_Obj[whitch_key].Press_Again = 0;
                Key_Obj[whitch_key].even = _KEY_EVEN_DOUBLECLICK;
                Key_Obj[whitch_key].Press_Assic = 0;

            }

        } else {//����Ϊ��һ�ΰ�ѹ���ð��������ʼ����

            Key_Obj[whitch_key].Press_Assic = 1;
            Key_Obj[whitch_key].Press_Again = 1;

        }
    } else {//�����û�е�����Ϊ�������ָ���һ��״̬
        Key_Obj[whitch_key].statue = _KEY_STATUS_DOWN;
    }
}

static void _KEY_Statue_Down_Handle(uint8_t whitch_key) {
    if (Key_Obj[whitch_key].Press_Again) {//������ѹ״̬�������ѹ�����򽫽��뵯�𶶶�״̬
        if (Key_Obj[whitch_key].Read_Key()
                != Key_Obj[whitch_key].Key_Down_Level) {
            Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASESHAKE;

        }
    } else {//�����жϰ�ѹʱ�䣬���������ֵ�����ж�Ϊ��������ձ�־
        if (++Key_Obj[whitch_key].Press_Long >= _KEY_LONG_TIME) {
            Key_Obj[whitch_key].even = _KEY_EVEN_LONGPRESS;
            Key_Obj[whitch_key].Press_Long = 0;
            Key_Obj[whitch_key].run_flag = 0;
            printf("key%d long press \n",whitch_key+1);
        } else if (Key_Obj[whitch_key].Read_Key()//����������ж�Ϊ����֮ǰ�͵�������뵯�𶶶�˫̥
                != Key_Obj[whitch_key].Key_Down_Level) {
            Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASESHAKE;
        }
    }
}

static void _KEY_Statue_DownShake_Handle(uint8_t whitch_key) {//���¶���״̬
    if (Key_Obj[whitch_key].Read_Key() == Key_Obj[whitch_key].Key_Down_Level) {
        Key_Obj[whitch_key].statue = _KEY_STATUS_DOWN;
    } else {
        Key_Obj[whitch_key].statue = _KEY_STATUS_RELEASE;
    }
}

static void _KEY_Press_Reset_Handle(uint8_t whitch_key) {//������λ���ȴ������������жϰ�ѹ�¼���run_flag���㣬�������״̬��
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

