#include "bsp_NRF24.h"
#include "stdio.h"


static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
/*-------------- SPI Operations ---------------*/

 /**
  * @brief  SPI Initialisation
  * @param  void
  * @retval void
  */

static void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组

	//Nrf2401中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

	
void SPI_NRF_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	Nvic_Init();
	
	/* Enable Clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// Configure SCK, MISO, MOSI
	GPIO_InitStructure.GPIO_Pin = NRF_SCK_Pin | NRF_MISO_Pin | NRF_MOSI_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Configure CE
	GPIO_InitStructure.GPIO_Pin = NRF_CE_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(NRF_CE_Port, &GPIO_InitStructure);
	
	// Configure CSN
	GPIO_InitStructure.GPIO_Pin = NRF_CSN_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(NRF_CSN_Port, &GPIO_InitStructure);
	
	// Configure IRQ
	GPIO_InitStructure.GPIO_Pin = NRF_IRQ_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(NRF_IRQ_Port, &GPIO_InitStructure);

	//irq
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//外部中断
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

  // Stop NRF
  SPI_NRF_CSN_H;

  /* SPI Mode Cofiguration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(NRF_SPI , &SPI_InitStructure);

  // Enable SPI
  SPI_Cmd(NRF_SPI , ENABLE);
	printf("\r\n SPI	Initialisation finished ...");
	
}

 /**
  * @brief  SPI basic Read/Write Operation
  * @param  data   :   Data to be written
  * @retval SPI status
  */

uint8_t SPI_RW(uint8_t data)
{
	uint32_t SPITimeout = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(NRF_SPI , SPI_I2S_FLAG_TXE) == RESET) // Wait while Send Bufer Empty
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
  }
	
	SPI_I2S_SendData(NRF_SPI , data);
	
	while (SPI_I2S_GetFlagStatus(NRF_SPI , SPI_I2S_FLAG_RXNE) == RESET) // Wait while Receive Bufer Empty
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
  }
	return SPI_I2S_ReceiveData(NRF_SPI);		
}

 /**
  * @brief  NRF Register Write
  * @param  reg   :   Register Addr
	* @param  value :   Data to be written
  * @retval SPI status
  */

uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
{
	 uint8_t status;
	 
	 SPI_NRF_CSN_L;
	 status = SPI_RW(reg);
	 // printf("\r\n status=%d", status);
	 SPI_RW(value);
	 SPI_NRF_CSN_H;
	 
	 return status;	 
}

 /**
  * @brief  NRF Register Read
  * @param  reg   :   Register Addr
  * @retval Retrived Data
  */

uint8_t NRF_Read_Reg(uint8_t reg)
{
	 uint8_t reg_val;
	 
	 SPI_NRF_CSN_L;
	 SPI_RW(reg);
	 reg_val = SPI_RW(0);
	 SPI_NRF_CSN_H;
	 
	 return reg_val;	 
}

 /**
  * @brief  NRF Bufer Write
  * @param  reg   : Register Addr
	* @param  *pBuf : Data
	* @param  uchars: The length of Data
  * @retval SPI status
  */

uint8_t NRF_Write_Buf(uint8_t reg, uint8_t * pBuf, uint8_t uchars)
{
	 uint8_t i;
	 uint8_t status;
	 
	 SPI_NRF_CSN_L;
	 status = SPI_RW(reg); // Write Register Addr
	 
	 for(i=0; i<uchars;i++)
	 {
			SPI_RW(pBuf[i]);   // Write Data
	 }
	 SPI_NRF_CSN_H;        // Stop NRF
	 
	 return status;	 
}

 /**
  * @brief  NRF Bufer Read
  * @param  reg   : Register Addr
	* @param  *pBuf : Read Data Bufer
	* @param  uchars: The length of Data
  * @retval SPI status
  */

static uint8_t NRF_Read_Buf(uint8_t reg, uint8_t * pBuf, uint8_t uchars)
{
	 uint8_t i;
	 uint8_t status;
	 
	 
	 SPI_NRF_CSN_L;
	 status = SPI_RW(reg);   // Write Register Addr
	 for(i=0;i<uchars;i++)
	 {
			pBuf[i] = SPI_RW(0);
	 }

	 SPI_NRF_CSN_H;
	 
	 return status;	 
}


 /**
  * @brief  Set the NRF24L01 to send mode
  * @param  void
  * @retval void
  */

static void NRF24L01_Set_TX(void)
{
	NRF_CE_L;
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0E);
	NRF_CE_H;
}

 /**
  * @brief  Set the NRF24L01 to receive mode
  * @param  void
  * @retval void
  */

static void NRF24L01_Set_RX(void)
{
	NRF_CE_L;
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0F);
	NRF_CE_H;
}
 /**
  * @brief  NRF24L01 Send Data
  * @param  tx_buf   : data
	* @param  len : the length of data
  * @retval void
  */

void NRF_Send_TX(uint8_t * tx_buf, uint8_t len)
{		
	NRF24L01_Set_TX();
	NRF_CE_L;//进入待机模式1	
	NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len);//装载数据
	NRF_CE_H;//设置CE为高，启动发射。CE高电平持续时间最小为10us
}


 /**
  * @brief  NRF24L01 Initialisation check function
	* @param  void
  * @retval void
  */

void NRF24L01_Check(void)
{ 
	uint8_t buf[5]; 
	uint8_t i; 

	NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR,TX_ADDRESS,5); 	// Write 5 Bytes data to NRF
	NRF_Read_Buf(NRF_READ_REG + TX_ADDR,buf,5);   				// Read 5 Bytes data from NRF
	
	// Compare sent and received data
	for(i=0;i<5;i++) 
	{ 
		if(buf[i]!=TX_ADDRESS[i])
		{
			break;
		}
		
	} 
	if(i==5)
		printf("\r\n NRF24L01 	Initialisation Successful ...");
	else
		printf("\r\n WARNING:  	NRF24L01 Initialisation Failed !");
}

/*---------------NRF24L01 Operations----------------*/
 /**
  * @brief  NRF24L01 Initialisation Function
  * @param  Channelx   : Channel
	* @param  Mode : Send mode (TX) or Receive mode (RX)
  * @retval void
  */
void NRF24L01_Init(uint8_t Channelx, uint8_t Mode)
{
	NRF_CE_L;
	
	NRF_Write_Reg(FLUSH_TX,0xff);  // Clean Send Bufer
	NRF_Write_Reg(FLUSH_RX,0xff);  // Clean Receive Bufer
	
	NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, 5);  		// Write TX Addr
	NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, 5);  	// Write RX Addr
	
	NRF_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);									// Enable auto-answer for channel 0
	NRF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);							// Enable receive addr for channel 0
	NRF_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);						// Set auto resend interval: 500us; Max. number of re-sends: 10
	NRF_Write_Reg(NRF_WRITE_REG + RF_CH, 40);							// Set RF channel
	NRF_Write_Reg(NRF_WRITE_REG + RX_PW_P0, 32);								// Set the effective data width of channel 0
	NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);							// Set TX transmit params, 0db gain, 2Mbps, enable low noise gain
	
	if(Mode == TX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0E);
	else if(Mode == RX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
	
	
	NRF_CE_H;

	NRF24L01_print_reg();
}


/**
  * @brief  Waiting overtime callback function
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  // 
  printf("SPI wait overtime!errorCode = %d",errorCode);
  return 0;
}

static void NRF24L01_Analyse(void)
{
	uint8_t sum = 0,i;
	uint8_t len = NRF24L01_RXDATA[3] + 5;
	int16_t joy_left_x_value = 0;
	int16_t joy_left_y_value = 0;

//	for(i=3;i<len;i++)
//		sum ^= NRF24L01_RXDATA[i];
//	if( sum!=NRF24L01_RXDATA[len] )	return;	//数据校验
//	if( NRF24L01_RXDATA[0] != '$' )	return;	//数据校验
//	if( NRF24L01_RXDATA[1] != 'M' )	return;	//数据校验
//	if( NRF24L01_RXDATA[2] != '<' )	return;	//MWC发送给上位机的标志

	joy_left_x_value = joy_left_x_value | NRF24L01_RXDATA[6];
	joy_left_x_value = joy_left_x_value << 8;
	joy_left_x_value = joy_left_x_value | NRF24L01_RXDATA[5];
	printf("%d, ", joy_left_x_value);

	joy_left_y_value = joy_left_y_value | NRF24L01_RXDATA[8];
	joy_left_y_value = joy_left_y_value << 8;
	joy_left_y_value = joy_left_y_value | NRF24L01_RXDATA[7];
	printf("%d \n", joy_left_y_value);
	
}

static void nRF24L01_Set_TX_Mode(uint8_t *TX_Data)
{
    NRF_CE_L;                                         // 待机（写寄存器之前一定要进入待机模式或掉电模式）
    NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, 5);    /*写寄存器指令+接收节点地址+地址宽度*/
    NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, 5); /*为了接收设备应答信号，接收通道0地址与发送地址相同*/
    NRF_Write_Buf(WR_TX_PLOAD, TX_Data, 32);              /*写有效数据地址+有效数据+有效数据宽度*/

    NRF_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);                            /*接收通道0自动应答*/
    NRF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);                       /*使能接收通道0*/
    NRF_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);                       /*自动重发延时250US+86US，重发10次*/
    NRF_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                              /*(2400)MHZ射频通道* 40是信道*/
    NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);                         /*1Mbps速率,发射功率:0DBM,低噪声放大器增益*/
    NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);                           /*发送模式,上电,16位CRC校验,CRC使能*/
    NRF_CE_H;
}

void nRF24L01_Set_RX_Mode(void)
{
    NRF_CE_L; // 待机
    // SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);
    NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, 5);
    NRF_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01); // auot ack
    NRF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);
    // SPI_W_Reg(W_REGISTER+SETUP_RETR,0x0a);
    NRF_Write_Reg(NRF_WRITE_REG + RX_PW_P0, 32);
    NRF_Write_Reg(NRF_WRITE_REG + RF_CH, 40);
    NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f); // 0db,lna
    NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);

    NRF_CE_H;
}

void NRF24L01_Test(void)
{
	NRF24L01_Analyse();
}

void NRF24L01_print_reg(void)
{
		uint8_t buf[8];
		for(int i = 0;i<1;i++)
		{
			NRF_Read_Buf(NRF_READ_REG + 0x07,buf,1);   				// Read 5 Bytes data from NRF
			printf("Buffer STATUS:");
			printf("%d \n", buf[0]);
		}
}

void NRF24L01_IRQ(void)	
{
	uint8_t status = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	if(status & (1<<RX_DR))//接收中断
	{	
		uint8_t rx_len = NRF_Read_Reg(R_RX_PL_WID);
		if(rx_len==32)
		{
			NRF_Read_Buf(RD_RX_PLOAD,NRF24L01_RXDATA,rx_len);//读取接收FIFO数据
			Nrf_Erro = 0;
		}
		else
		{
			NRF_Write_Reg(FLUSH_RX,0xff);//清空接收缓冲区
		}
	}
	if(status & (1<<MAX_RT))//达到最多次重发中断
	{
		if(status & (1<<TX_FULL))//TX FIFO 溢出
		{
			NRF_Write_Reg(FLUSH_TX,0xff);//清空发送缓冲区
		}
	}
//	if(status&(1<<TX_DS))//发送完成
//	{	
		NRF24L01_Set_RX();//设置Nrf2401为接收模式
//	}
	NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, status);//清除中断标志位	
}