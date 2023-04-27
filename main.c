/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             main
* @company          �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/



#include "zf_common_headfile.h"
#include "LED.h"
#include "KEY.h"
//#include "PWM.h"
#include "character.h"
#include "menu.h"
#include "game_bird.h"
#include "snake.h"
#include "NRF.h"

uint16_t Num;
uint16_t Lim;
uint8 i;
uint j=1;
uint8 uart_flag;
uint8 grade;

uint8 RX_Data[200];

 const MenuInit menu[7]={{0,"GAME",1,3},{0,"TIMER",2,0},{1,"LED",2,5},
{0,"SNAKE",4,3},{3,"GRADE",4,4},{2,"PWM",6,5},{5,"NORMAL",6,6}};

//extern FlappyBird bird;
extern uint8 BirdPos[][16];
int main(void)
{
    uint8 string[]="SUCCESSFUL";
    int sel=0;
    interrupt_global_disable();             // �ر����ж�
    clock_init(SYSTEM_CLOCK_144M);          // ��ر���������ϵͳʱ�ӡ�
    debug_init();                           // ��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    // �˴���д�û�����(���磺�����ʼ�������)
    KEY_Init();
    oled_init();
    interrupt_global_enable();              // ���ж������
    oled_clear();
    LED_Mode3_Init();
    pwm_set_duty(TIM1_PWM_CH1_A8,5000);
    pwm_set_duty(TIM1_PWM_CH2_A9,5000);
    pwm_set_duty(TIM1_PWM_CH3_A10,5000);
    pwm_set_duty(TIM1_PWM_CH4_A11,5000);
    NRF24L01_Init();
    oled_show_int(0, 0, 1, 1);
    while(1)
    {
        if(NRF24L01_Check()==0)
            {
                oled_show_int(0, 0, 2, 1);
                //NRF24L01_TX_Mode();
            }
    }
}








