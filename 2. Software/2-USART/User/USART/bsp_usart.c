#include "bsp_usart.h"
#include "stdio.h"


static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/*----1. CLOCK of GPIO & USART----*/

	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	RCC_APB2PeriphClockCmd(USART_CLK, ENABLE);
	
	/*----2. Configure GPIO of input and output----*/

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure);
	

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/*----3. Configure USART----*/
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = baudrate;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART1, &USART_InitStructure);
	
	/*----4. Enable USART Interruption----*/
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	
	/*----5. Enable USART----*/
	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);

	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)      
{
	 // Send a Byte to USART
	 USART_SendData(pUSARTx, ch);
	 
	 while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data)
{
	uint8_t temp_h, temp_l;
	
	temp_h = (data&0xff00) >> 8;
	temp_l = data&0xff;
	
	USART_SendData(pUSARTx, temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(pUSARTx, temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
}

void Usart_SendString(USART_TypeDef *pUSARTx, char *str)     
{
	 unsigned int k=0;
	 
	 do{
			Usart_SendByte(pUSARTx, *(str+k));
			k++;
	 }while (*(str+k) != '\0'); // Enpty char
	 
	 
	 while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET){}
}

// redirect the printf and scanf function
int fputc(int ch, FILE *f)
{
	/* transmit a byte to USART*/
	USART_SendData(USART1, (uint8_t) ch);
	
	/* Awaiting for transmitting */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	return (ch);
}

// redirect the scanf function to USART, after redirecting you can use scanf() and getchar() etc.
int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)  == RESET); 
	
	return (int)USART_ReceiveData(USART1);
}
