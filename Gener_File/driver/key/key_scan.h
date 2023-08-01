/*
 * key_scan.h
 *
 *  Created on: 2023年7月4日
 *      Author: Administrator
 */

#ifndef _KEY_KEY_SCAN_H_
#define _KEY_KEY_SCAN_H_
#include "debug.h"


//定义用户按键为PA0
#define KEY_USER_READ() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY_DOWN 0
#define KEY_UP 1

/*
 * 定义按键状态枚举类型
 *
 *
 *
 * */
typedef enum
{
    FSM_KEY_Up,//按键释放
    FSM_KEY_DownShake,//按键按压抖动
    FSM_KEY_Down,//按键按压状态
    FSM_KEY_UpShake,//按键释放抖动
}FSM_State_t;

/**
 * 定义按键值结构体，包含按键状态，按键扫描计数值，按键辅助标志
 */
typedef struct{
    FSM_State_t state;//按键状态
    uint8_t volatile cnt;//按键计数值
    uint8_t auxiliary_flag;//按键辅助标志，判断按键是否按下过
}FSM_value_t;

typedef enum
{
    KEY_RELESS = 0,//按键释放
    KEY_CHICK,//按键单击
    KEY_DOUBLE,//按键双击
    KEY_PRESS,//按键长按

}KEY_action_t;

/**
 * 按键id，为用户定义的按键id，其中KEY_NULL必须存在且为枚举值为0
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
 * 按键id结构体
 */
typedef struct
{
    KEY_id_t id;//保存按键id
    KEY_action_t action;
}KEY_t;

extern FSM_value_t FSM_value;
extern KEY_t KEY_whitch;
void key_scan();
void Key_Init();
void Key_Action();
#endif /* GENER_FILE_DRIVER_KEY_KEY_SCAN_H_ */
