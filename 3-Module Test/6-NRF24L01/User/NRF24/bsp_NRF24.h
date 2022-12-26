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

#define RX_DR						6				//���������ж�.�����յ���Ч���ݺ���1��д��1������жϡ� 
#define TX_DS						5				//���ݷ�������жϡ������ݷ�����ɺ�����жϡ�����������Զ�Ӧ��ģʽ�£�ֻ�е����յ�Ӧ���źź��λ��1��д��1������жϡ� 
#define MAX_RT					4				//�ﵽ�����ط��жϡ�д��1������жϡ����MAX_RT�жϲ�������������ϵͳ���ܽ���ͨѶ��
#define TX_FULL 				0				//TX FIFO�Ĵ�������־��   1:TX FIFO  �Ĵ�����   0: TX FIFO �Ĵ���δ��, �п��ÿռ䡣


// NRF24L01�Ĵ���ָ��
#define NRF_READ_REG    0x00  	// ���Ĵ���ָ��
#define NRF_WRITE_REG   0x20 		// д�Ĵ���ָ��
#define ACTIVATE				0x50  	// follow with 0x73 to activate feature register
#define R_RX_PL_WID   	0x60		// �����ջ������ĳ���
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define W_ACK_PAYLOAD		0xA8		// Used in RX mode.
#define FLUSH_TX        0xE1 		// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����

// NRF24L01�Ĵ�����ַ
#define CONFIG          0x00  	// �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  	// �Զ�Ӧ��������
#define EN_RXADDR       0x02  	// �����ŵ�����
#define SETUP_AW        0x03  	// �շ���ַ�������
#define SETUP_RETR      0x04  	// �Զ��ط���������
#define RF_CH           0x05  	// ����Ƶ������
#define RF_SETUP        0x06  	// �������ʡ����Ĺ�������
#define NRFRegSTATUS    0x07  	// ״̬�Ĵ���
#define OBSERVE_TX      0x08  	// ���ͼ�⹦��
#define CD              0x09  	// ��ַ���           
#define RX_ADDR_P0      0x0A  	// Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  	// Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  	// Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  	// Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  	// Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  	// Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  	// ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  	// ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  	// ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  	// ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  	// ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  	// ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  	// ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  	// FIFOջ��ջ��״̬�Ĵ�������

#define DYNPD    				0x1C  	// per pipe DPL control 
#define FEATURE    			0x1D  	// ��Feature�� register address 

#endif 

