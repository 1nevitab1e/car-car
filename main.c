/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             main
* @company          成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
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
//#include "daojishi.h"

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
    uint8 string[]="FUCK";
    int sel=0;
    interrupt_global_disable();             // 关闭总中断
    clock_init(SYSTEM_CLOCK_144M);          // 务必保留，设置系统时钟。
    debug_init();                           // 务必保留，本函数用于初始化MPU 时钟 调试串口

    // 此处编写用户代码(例如：外设初始化代码等)
    KEY_Init();
    oled_init();
    interrupt_global_enable();              // 总中断最后开启
    uart_init(UART_3, 115200, UART3_TX_B10, UART3_RX_B11);
    uart_rx_interrupt(UART_3,ENABLE);
    oled_clear();
    oled_show_string(1, 0, "RX_DATA");
//    oled_show_string(1, 2, "TX_DATA");
    //timer_init();
    //oled_show_string(1, 1, menu[sel].content);
    //int i=0;
    LED_Mode3_Init();
    pwm_set_duty(TIM1_PWM_CH1_A8,5000);
    pwm_set_duty(TIM1_PWM_CH2_A9,5000);
    pwm_set_duty(TIM1_PWM_CH3_A10,5000);
    pwm_set_duty(TIM1_PWM_CH4_A11,5000);
    printf("%s\r\n",string);
    //uart_write_string(UART_3,"Start:");
    while(1)
    {
        if(uart_flag == 1)
        {
            oled_show_string(1, 1, "          ");
            oled_show_string(1, 1, RX_Data);
            printf("%s\r\n",RX_Data);
            uart_flag=0;
        }
        //system_delay_ms(1000);
        //scanf();
        //i=Key_Get();
        //Menu_Sel(&i, &sel);
    }
}








