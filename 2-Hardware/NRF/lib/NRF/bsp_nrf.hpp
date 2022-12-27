#ifndef __BSP_NRF_HPP
#define __BSP_NRF_HPP

#include <Arduino.h>

#define uint unsigned int
#define uchar unsigned char

#define TX_ADDR_WITDH 5 // 发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5
#define TX_DATA_WITDH 32
#define RX_DATA_WITDH 32
#define R_REGISTER 0x00  // 读寄存器
#define W_REGISTER 0x20  // 写寄存器
#define R_RX_PLOAD 0x61  // 读RX FIFO有效数据，1-32字节，当读数据完成后，数据被清除，应用于接收模式
#define W_TX_PLOAD 0xA0  // 写TX FIFO有效数据，1-32字节，写操作从字节0开始，应用于发射模式
#define FLUSH_TX 0xE1    // 清除TX FIFO寄存器，应用于发射模式
#define FLUSH_RX 0xE2    // 清除RX FIFO寄存器，应用于接收模式
#define REUSE_TX_PL 0xE3 // 重新使用上一包有效数据，当CE为高过程中，数据包被不断的重新发射
// #define NOP 0xFF         // 空操作，可以用来读状态寄存器
/******************************************************************
// nRF24L01寄存器地址
*******************************************************************/
#define CONFIG 0x00      // 配置寄存器
#define EN_AA 0x01       // “自动应答”功能寄存器
#define EN_RX_ADDR 0x02  // 接收通道使能寄存器
#define SETUP_AW 0x03    // 地址宽度设置寄存器
#define SETUP_RETR 0x04  // 自动重发设置寄存器
#define RF_CH 0x05       // 射频通道频率设置寄存器
#define RF_SETUP 0x06    // 射频设置寄存器
#define STATUS 0x07      // 状态寄存器
#define OBSERVE_TX 0x08  // 发送检测寄存器
#define CD 0x09          // 载波检测寄存器
#define RX_ADDR_P0 0x0A  // 数据通道0接收地址寄存器
#define RX_ADDR_P1 0x0B  // 数据通道1接收地址寄存器
#define RX_ADDR_P2 0x0C  // 数据通道2接收地址寄存器
#define RX_ADDR_P3 0x0D  // 数据通道3接收地址寄存器
#define RX_ADDR_P4 0x0E  // 数据通道4接收地址寄存器
#define RX_ADDR_P5 0x0F  // 数据通道5接收地址寄存器
#define TX_ADDR 0x10     // 发送地址寄存器
#define RX_PW_P0 0x11    // 数据通道0有效数据宽度设置寄存器
#define RX_PW_P1 0x12    // 数据通道1有效数据宽度设置寄存器
#define RX_PW_P2 0x13    // 数据通道2有效数据宽度设置寄存器
#define RX_PW_P3 0x14    // 数据通道3有效数据宽度设置寄存器
#define RX_PW_P4 0x15    // 数据通道4有效数据宽度设置寄存器
#define RX_PW_P5 0x16    // 数据通道5有效数据宽度设置寄存器
#define FIFO_STATUS 0x17 // FIFO状态寄存器
//*********************************************************************************
extern uchar nrf_sta;                  // 状态变量
#define RX_DR (nrf_sta & 0x40)  // 接收成功中断标志
#define TX_DS (nrf_sta & 0x20)  // 发射成功中断标志
#define MAX_RT (nrf_sta & 0x10) // 重发溢出中断标志
#define ADC_Mid 2100

/******************************************************************
// DECLARE CONSTANTs
*******************************************************************/
extern int CE;
extern int CSN;
extern int IRQ;
// int SCK = 13;
// int MOSI = 11;
// int MISO = 12;  这几个引脚是硬件固定的，无法更改
extern uchar TX_Addr[];
extern uchar RX_Addr[];
extern uchar TX_Buffer[];
extern uchar RX_Buffer[RX_DATA_WITDH];
extern char flag;
extern int data_x;
extern unsigned long df;
extern unsigned long t;

/******************************************************************
// DECLARE FUNCTIONs
*******************************************************************/
void nRF24L01_Init(void);
uchar SPI_RW(uchar byte);
uchar SPI_W_Reg(uchar reg, uchar value);
uchar SPI_R_byte(uchar reg);
uchar SPI_R_DBuffer(uchar reg, uchar *Dat_Buffer, uchar Dlen);
uchar SPI_W_DBuffer(uchar reg, uchar *TX_Dat_Buffer, uchar Dlen);
void nRF24L01_Set_TX_Mode(uchar *TX_Data);
uchar Check_Ack(void);
uchar Check_Rec(void);
void nRF24L01_Set_RX_Mode(void);
uchar nRF24L01_RX_Data(void);
void NRF24L01_print_reg(void);

#endif
