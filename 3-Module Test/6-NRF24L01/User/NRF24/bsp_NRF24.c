#include "bsp_NRF24.h"
#include "stdio.h"


static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
/*-------------- SPI Operations ---------------*/

 /**
  * @brief  SPI Initialisation
  * @param  void
  * @retval void
  */
	
void SPI_NRF_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
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
	NRF_Write_Reg(NRF_WRITE_REG + RF_CH, Channelx);							// Set RF channel
	NRF_Write_Reg(NRF_WRITE_REG + RX_PW_P0, 32);								// Set the effective data width of channel 0
	NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);							// Set TX transmit params, 0db gain, 2Mbps, enable low noise gain
	
	if(Mode == TX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0E);
	else if(Mode == RX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
	
	
	NRF_CE_H;
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
	NRF24L01_Set_TX(); 		// Send mode
	NRF_CE_L; 						// Enter standby mode 1	
	NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len);// load data
	NRF_CE_H;							//Set CE high to start send, CE high for a minimum duration of 10us
	while(SPI_NRF_IRQ_Read);
	
	uint8_t status = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	
	if(status & (1<<MAX_RT)) //Maximum number of re-send interruptions reached
	{
		if(status & (1<<TX_FULL))//TX FIFO overflow
		{
			NRF_Write_Reg(FLUSH_TX,0xff); //clean send Bufer
			//printf("clean send buffer");
		}
	}
	if(status &(1<<TX_DS))//send finished
	{	
		NRF24L01_Set_RX();//set to receive mode
		printf("set to receive mode");
	}
	
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
	//uint8_t i=0;

	//printf(" receive some params %d", NRF24L01_RXDATA[5]);
	for(i=3;i<len;i++)
		sum ^= NRF24L01_RXDATA[i];
	if( sum!=NRF24L01_RXDATA[len] )	return;	//����У��
	if( NRF24L01_RXDATA[0] != '$' )	return;	//����У��
	if( NRF24L01_RXDATA[1] != 'M' )	return;	//����У��
	if( NRF24L01_RXDATA[2] != '>' )	return;	//MWC���͸���λ���ı�־
}

static void nRF24L01_Set_TX_Mode(uint8_t *TX_Data)
{
    NRF_CE_L;                                         // ������д�Ĵ���֮ǰһ��Ҫ�������ģʽ�����ģʽ��
    NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, 5);    /*д�Ĵ���ָ��+���սڵ��ַ+��ַ���*/
    NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, 5); /*Ϊ�˽����豸Ӧ���źţ�����ͨ��0��ַ�뷢�͵�ַ��ͬ*/
    NRF_Write_Buf(WR_TX_PLOAD, TX_Data, 32);              /*д��Ч���ݵ�ַ+��Ч����+��Ч���ݿ��*/

    NRF_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);                            /*����ͨ��0�Զ�Ӧ��*/
    NRF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);                       /*ʹ�ܽ���ͨ��0*/
    NRF_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);                       /*�Զ��ط���ʱ250US+86US���ط�10��*/
    NRF_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                              /*(2400)MHZ��Ƶͨ��* 40���ŵ�*/
    NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);                         /*1Mbps����,���书��:0DBM,�������Ŵ�������*/
    NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);                           /*����ģʽ,�ϵ�,16λCRCУ��,CRCʹ��*/
    NRF_CE_H;
}

void NRF24L01_Test(void)
{
	uint8_t test_data[8] = {0xfe, 100, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
	nRF24L01_Set_TX_Mode(test_data);
//	NRF24L01_Analyse();
//	//while(SPI_NRF_IRQ_Read);
//	//uint8_t status = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
//	NRF_Read_Buf(RD_RX_PLOAD,NRF24L01_RXDATA,32);
//	for(int i=0;i<32;i++)
//	{
//		if(NRF24L01_RXDATA[i] != 0)	
//		printf("%d", NRF24L01_RXDATA[i]);
//	}
//  //NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, status);
}