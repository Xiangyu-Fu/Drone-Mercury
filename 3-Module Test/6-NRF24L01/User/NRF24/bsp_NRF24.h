#ifndef __BSP_NRF24_H
#define __BSP_NRF24_H

#include "stm32f10x.h"

/*********************************************
             1. Definitions of SPI
**********************************************/ 
#define NRF_SPI												SPI1

#define NRF_SCK_Port									GPIOA
#define NRF_SCK_Pin										GPIO_Pin_5
#define NRF_MISO_Pin									GPIO_Pin_6
#define NRF_MOSI_Pin									GPIO_Pin_7

#define NRF_CE_Port       						GPIOB
#define NRF_CE_Pin										GPIO_Pin_2

#define NRF_CSN_Port      						GPIOA
#define NRF_CSN_Pin										GPIO_Pin_4

#define NRF_IRQ_Port      						GPIOB
#define NRF_IRQ_Pin										GPIO_Pin_0

#define SPI_NRF_CSN_L     						GPIO_ResetBits(NRF_CSN_Port, NRF_CSN_Pin)
#define SPI_NRF_CSN_H    							GPIO_SetBits(NRF_CSN_Port, NRF_CSN_Pin)

#define NRF_CE_L     									GPIO_ResetBits(NRF_CE_Port, NRF_CE_Pin)
#define NRF_CE_H    									GPIO_SetBits(NRF_CE_Port, NRF_CE_Pin)

#define SPI_NRF_IRQ_Read 							NRF_IRQ_Port->IDR  & NRF_IRQ_Pin


// Waiting Time
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


// Definitions of NRF
extern uint8_t NRF24L01_RXDATA[8];
extern uint8_t NRF24L01_TXDATA[8];
static uint8_t TX_ADDRESS[5] = {0x41, 0x41, 0x41, 0x41, 0x41};  // local Addr
static uint8_t RX_ADDRESS[5] = {0x42, 0x42, 0x42, 0x42, 0x42};  // Receive Addr


// Global function definitions
void SPI_NRF_Init(void);
void NRF24L01_Init(uint8_t Channelx, uint8_t Mode);
void NRF24L01_Check(void);
void NRF_Send_TX(uint8_t * tx_buf, uint8_t len);
void NRF24L01_Test(void);

uint8_t NRF_Read_Reg(uint8_t reg);
uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value);
uint8_t NRF_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);




/********************************************
         2. Definitions of NRF24L01
********************************************/


#define TX							1
#define RX							2

#define RX_DR						6				//接收数据中断.当接收到有效数据后置1。写‘1’清除中断。 
#define TX_DS						5				//数据发送完成中断。当数据发送完成后产生中断。如果工作在自动应答模式下，只有当接收到应答信号后此位置1。写‘1’清除中断。 
#define MAX_RT					4				//达到最多次重发中断。写‘1’清除中断。如果MAX_RT中断产生则必须清除后系统才能进行通讯。
#define TX_FULL 				0				//TX FIFO寄存器满标志。   1:TX FIFO  寄存器满   0: TX FIFO 寄存器未满, 有可用空间。


// NRF24L01寄存器指令
#define NRF_READ_REG    0x00  	// 读寄存器指令
#define NRF_WRITE_REG   0x20 		// 写寄存器指令
#define ACTIVATE				0x50  	// follow with 0x73 to activate feature register
#define R_RX_PL_WID   	0x60		// 读接收缓冲区的长度
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define W_ACK_PAYLOAD		0xA8		// Used in RX mode.
#define FLUSH_TX        0xE1 		// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留

// NRF24L01寄存器地址
#define CONFIG          0x00  	// 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  	// 自动应答功能设置
#define EN_RXADDR       0x02  	// 可用信道设置
#define SETUP_AW        0x03  	// 收发地址宽度设置
#define SETUP_RETR      0x04  	// 自动重发功能设置
#define RF_CH           0x05  	// 工作频率设置
#define RF_SETUP        0x06  	// 发射速率、功耗功能设置
#define NRFRegSTATUS    0x07  	// 状态寄存器
#define OBSERVE_TX      0x08  	// 发送监测功能
#define CD              0x09  	// 地址检测           
#define RX_ADDR_P0      0x0A  	// 频道0接收数据地址
#define RX_ADDR_P1      0x0B  	// 频道1接收数据地址
#define RX_ADDR_P2      0x0C  	// 频道2接收数据地址
#define RX_ADDR_P3      0x0D  	// 频道3接收数据地址
#define RX_ADDR_P4      0x0E  	// 频道4接收数据地址
#define RX_ADDR_P5      0x0F  	// 频道5接收数据地址
#define TX_ADDR         0x10  	// 发送地址寄存器
#define RX_PW_P0        0x11  	// 接收频道0接收数据长度
#define RX_PW_P1        0x12  	// 接收频道1接收数据长度
#define RX_PW_P2        0x13  	// 接收频道2接收数据长度
#define RX_PW_P3        0x14  	// 接收频道3接收数据长度
#define RX_PW_P4        0x15  	// 接收频道4接收数据长度
#define RX_PW_P5        0x16  	// 接收频道5接收数据长度
#define FIFO_STATUS     0x17  	// FIFO栈入栈出状态寄存器设置

#define DYNPD    				0x1C  	// per pipe DPL control 
#define FEATURE    			0x1D  	// “Feature” register address 

#endif 

