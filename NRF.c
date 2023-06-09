/*
 * NRF.c
 *
 *  Created on: 2023年4月14日
 *      Author: 86131
 */

#include "NRF.h"
#include "NRF_Config.h"

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址

void NRF24L01_Init(void)
{
    simu_spi_gpio_init();
    //system_delay_us()
    gpio_set_level(NRF24L01_CE,0);//使能NRF
    simu_spi_stop();//spi暂停传输
}

//检测24L01是否存在
//返回值:0，成功;1，失败
u8 NRF24L01_Check(void)
{
    u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
    u8 i;
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.
    NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址
    oled_show_int(0, 0, buf[0], 1);
    oled_show_int(0, 1, buf[1], 1);
    oled_show_int(0, 2, buf[2], 1);
    oled_show_int(0, 3, buf[3], 1);
    oled_show_int(0, 4, buf[4], 1);
    for(i=0;i<5;i++)if(buf[i]!=0XA5)break;
    if(i!=5)return 1;//检测24L01错误
    return 0;        //检测到24L01
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status,u8_ctr;
    simu_spi_start();          //使能SPI传输
    status = simu_spi_swapbyte(reg);//发送寄存器值(位置),并读取状态值
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)simu_spi_swapbyte(*pBuf++); //写入数据
    simu_spi_stop();      //关闭SPI传输
    return status;          //返回读到的状态值
}

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
    u8 status,u8_ctr;
    simu_spi_start();           //使能SPI传输
    status=simu_spi_swapbyte(reg);//发送寄存器值(位置),并读取状态值
    for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=simu_spi_swapbyte(0XFF);//读出数据
    simu_spi_stop();      //关闭SPI传输
    return status;        //返回读到的状态值
}

//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
    u8 status;
    simu_spi_start();                 //使能SPI传输
    status =simu_spi_swapbyte(reg);//发送寄存器号
    simu_spi_swapbyte(value);      //写入寄存器的值
    simu_spi_stop();                //禁止SPI传输
    return status;                 //返回状态值
}

//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;
    simu_spi_start();         //使能SPI传输
    simu_spi_swapbyte(reg);   //发送寄存器号
    reg_val=simu_spi_swapbyte(0XFF);//读取寄存器内容
    simu_spi_stop();          //禁止SPI传输
    return reg_val;           //返回状态值
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    simu_spi_start();
    gpio_set_level(NRF24L01_CE,0);//使能配置
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
    gpio_set_level(NRF24L01_CE,1);//启动发送
    while(NRF24L01_IRQ!=0);//等待发送完成
    sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
    if(sta&MAX_TX)//达到最大重发次数
    {
        NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器
        return MAX_TX;
    }
    if(sta&TX_OK)//发送完成
    {
        return TX_OK;
    }
    return 0xff;//其他原因发送失败
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
    u8 sta;
    sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
    if(sta&RX_OK)//接收到数据
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
        NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器
        return 0;
    }
    return 1;//没收到任何数据
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void NRF24L01_RX_Mode(void)
{
    gpio_set_level(NRF24L01_CE,0);
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址

    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);       //使能通道0的自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //使能通道0的接收地址
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);         //设置RF通信频率
    NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);
    //设置TX发射参数,0db增益,2Mbps,低噪声增益开启

    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);
    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式

    gpio_set_level(NRF24L01_CE,1); //CE为高,进入接收模式
}

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void NRF24L01_TX_Mode(void)
{
    gpio_set_level(NRF24L01_CE,0);
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);
   //设置TX节点地址,主要为了使能ACK

    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址
    NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);
    //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次

    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);
    //设置TX发射参数,0db增益,2Mbps,低噪声增益开启

    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);
    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断

    gpio_set_level(NRF24L01_CE,1);//CE为高,10us后启动发送
}
