/*
 * key_scan.h
 *
 *  Created on: 2023��7��4��
 *      Author: Administrator
 */

#ifndef _KEY_KEY_SCAN_H_
#define _KEY_KEY_SCAN_H_
#include "debug.h"



//���峤��ʱ���Լ�˫�����ʱ��
#define _KEY_LONG_TIME 100
#define _KEY_INTERVAL_TIME 60

//�����û�����ΪPA0
typedef enum _KEY_LIST //�����б�
{
    KEY1,
    KEY2,
//    KEY3,
    KEY_NUM,
}KEY_LIST;


typedef enum _KEY_STATUS_LIST // ����״̬
{
    _KEY_STATUS_RELEASE = 0x00,//�����ͷ�״̬
    _KEY_STATUS_DOWNSHAKE = 0x01,//�������¶���״̬
    _KEY_STATUS_DOWN = 0x02,        //��������״̬
    _KEY_STATUS_RELEASESHAKE = 0x04,//�����ͷŶ���״̬
}KEY_STATUS_LIST;

typedef enum _KEY_EVEN_LIST //�����¼�
{
    _KEY_EVEN_NULL = 0x00,//�����޶���
    _KEY_EVEN_CLICK = 0x01,//��������
    _KEY_EVEN_DOUBLECLICK = 0x02,//����˫��
    _KEY_EVEN_LONGPRESS = 0x04,//��������
}KEY_EVEN_LIST;



typedef struct _KEY_OBJECT
{
    uint8_t Press_Interval;   //���������־
    uint8_t Press_Long;       //����������־
    uint8_t Press_Again;      //���¹���־
    uint8_t Press_Assic;        //������־
    uint8_t Key_Down_Level;
    uint8_t run_flag;//״̬�������б�־ Ϊ1��ʾ�����¼��ȴ�����
    uint8_t (*Read_Key)(void);

    KEY_STATUS_LIST statue;
    KEY_EVEN_LIST even;

}KEY_OBJECT;
extern KEY_OBJECT Key_Obj[KEY_NUM];


void KEY_Init(void);
void KEY_StatuMachine();

#endif /* GENER_FILE_DRIVER_KEY_KEY_SCAN_H_ */
