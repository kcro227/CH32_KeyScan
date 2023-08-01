/*
 * key_scan.h
 *
 *  Created on: 2023��7��4��
 *      Author: Administrator
 */

#ifndef _KEY_KEY_SCAN_H_
#define _KEY_KEY_SCAN_H_
#include "debug.h"


//�����û�����ΪPA0
#define KEY_USER_READ() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY_DOWN 0
#define KEY_UP 1

/*
 * ���尴��״̬ö������
 *
 *
 *
 * */
typedef enum
{
    FSM_KEY_Up,//�����ͷ�
    FSM_KEY_DownShake,//������ѹ����
    FSM_KEY_Down,//������ѹ״̬
    FSM_KEY_UpShake,//�����ͷŶ���
}FSM_State_t;

/**
 * ���尴��ֵ�ṹ�壬��������״̬������ɨ�����ֵ������������־
 */
typedef struct{
    FSM_State_t state;//����״̬
    uint8_t volatile cnt;//��������ֵ
    uint8_t auxiliary_flag;//����������־���жϰ����Ƿ��¹�
}FSM_value_t;

typedef enum
{
    KEY_RELESS = 0,//�����ͷ�
    KEY_CHICK,//��������
    KEY_DOUBLE,//����˫��
    KEY_PRESS,//��������

}KEY_action_t;

/**
 * ����id��Ϊ�û�����İ���id������KEY_NULL���������Ϊö��ֵΪ0
 */
typedef enum
{
    KEY_NULL = 0,
//    KEY_DOWN,
//    KEY_ENTER,
//    KEY_UP,
    KEY_USER,
}KEY_id_t;
/**
 * ����id�ṹ��
 */
typedef struct
{
    KEY_id_t id;//���水��id
    KEY_action_t action;
}KEY_t;

extern FSM_value_t FSM_value;
extern KEY_t KEY_whitch;
void key_scan();
void Key_Init();
void Key_Action();
#endif /* GENER_FILE_DRIVER_KEY_KEY_SCAN_H_ */
