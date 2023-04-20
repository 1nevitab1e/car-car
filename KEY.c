/*
 * KEY.c
 *
 *  Created on: 2023Äê3ÔÂ24ÈÕ
 *      Author: 86131
 */

#include "zf_common_headfile.h"

void KEY_Init()
{
    gpio_init(C2, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(C1, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(C3, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(C4, GPI, GPIO_HIGH, GPI_PULL_UP);
}

int Key_Get()
{
    int i=0;

    if(gpio_get_level(C2) == 0)
        {
            system_delay_ms(10);
            while(gpio_get_level(C2) == 0);
            system_delay_ms(10);
            i=1;
        }
    if(gpio_get_level(C1) == 0)
        {
            system_delay_ms(10);
            while(gpio_get_level(C1) == 0);
            system_delay_ms(10);
            i=2;
        }
    if(gpio_get_level(C3) == 0)
        {
            system_delay_ms(10);
            while(gpio_get_level(C3) == 0);
            system_delay_ms(10);
            i=3;
        }
    if(gpio_get_level(C4) == 0)
        {
            system_delay_ms(10);
            while(gpio_get_level(C4) == 0);
            system_delay_ms(10);
            i=4;
        }
    return i;
}
