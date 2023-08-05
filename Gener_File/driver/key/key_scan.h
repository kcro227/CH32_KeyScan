/*
 * key_scan.h
 *
 *  Created on: 2023年7月4日
 *      Author: Administrator
 */

#ifndef _KEY_KEY_SCAN_H_
#define _KEY_KEY_SCAN_H_
#include "debug.h"



//定义长按时间以及双击间隔时间
#define _KEY_LONG_TIME 100
#define _KEY_INTERVAL_TIME 60

//定义用户按键为PA0
typedef enum _KEY_LIST //按键列表
{
    KEY1,
    KEY2,
//    KEY3,
    KEY_NUM,
}KEY_LIST;


typedef enum _KEY_STATUS_LIST // 按键状态
{
    _KEY_STATUS_RELEASE = 0x00,//按键释放状态
    _KEY_STATUS_DOWNSHAKE = 0x01,//按键按下抖动状态
    _KEY_STATUS_DOWN = 0x02,        //按键按下状态
    _KEY_STATUS_RELEASESHAKE = 0x04,//按键释放抖动状态
}KEY_STATUS_LIST;

typedef enum _KEY_EVEN_LIST //按键事件
{
    _KEY_EVEN_NULL = 0x00,//按键无动作
    _KEY_EVEN_CLICK = 0x01,//按键单击
    _KEY_EVEN_DOUBLECLICK = 0x02,//按键双击
    _KEY_EVEN_LONGPRESS = 0x04,//按键长按
}KEY_EVEN_LIST;



typedef struct _KEY_OBJECT
{
    uint8_t Press_Interval;   //间隔计数标志
    uint8_t Press_Long;       //长按计数标志
    uint8_t Press_Again;      //按下过标志
    uint8_t Press_Assic;        //辅助标志
    uint8_t Key_Down_Level;
    uint8_t run_flag;//状态机可运行标志 为1表示长按事件等待弹起
    uint8_t (*Read_Key)(void);

    KEY_STATUS_LIST statue;
    KEY_EVEN_LIST even;

}KEY_OBJECT;
extern KEY_OBJECT Key_Obj[KEY_NUM];


void KEY_Init(void);
void KEY_StatuMachine();

#endif /* GENER_FILE_DRIVER_KEY_KEY_SCAN_H_ */
