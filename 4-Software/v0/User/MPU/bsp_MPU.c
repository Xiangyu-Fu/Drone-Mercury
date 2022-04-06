#include "bsp_MPU.h"
#include "stdio.h"

uint32_t MPU6050_Buffer[14];
uint32_t MPU6050_Data[7];

//////////////// STATIC FUNCS /////////////////////
static void delay(uint32_t count)
{
	u32 i;
	for(i=0;i<count;i++);
}

/*--------I2C Start & Stop Func--------*/
static uint8_t I2C_Start(void)
{
  SCL_L;
	SDA_H;
	SCL_H;
	__nop();
	if(!SDA_Read)
	{
		return 0;
	}
	SDA_L;
	__nop();
	if(SDA_Read)
	{
		return 0;
	}
	SDA_L;
	return 1;
}

static void I2C_Stop(void)
{
	SCL_L;
	SDA_L;
	SCL_H;
	SDA_H;
}


/*-------Send & Receive Data Func------*/
// Send 1 Byte Data
static void I2C_WriteByte(unsigned char SendByte)
{
	uint8_t i=8;
	while(i--)
	{
		SCL_L;
		if(SendByte & 0x80)SDA_H;
		else SDA_L;
		SendByte <<= 1;
		SCL_H;
	}
	SCL_L;
}

// Read 1 Byte Data
static uint8_t I2C_ReadByte(void)
{
	uint8_t i = 8;
	uint8_t ReceivedByte = 0x00;
	SDA_H; // release SDA, i.e. set SDA at high voltage.
	__nop();
	while(i--)
	{
		SCL_L;
		__nop();
		__nop();
		SCL_H;
		__nop();
		ReceivedByte <<= 1;
		__nop();
		if(SDA_Read) 
			ReceivedByte |= 0x01;
	}
	SCL_L;
	// printf("\r\n test:%d", ReceivedByte);
	return ReceivedByte;
}

/*--------Master receive ACK & NACK Signal--------*/
static uint8_t I2C_WaitAck(void)
{
	SCL_L;
	__nop();
	SDA_H; // release SDA
	__nop();
	SCL_H;
	__nop();
	if(SDA_Read) // If receive the Nack
	{
		SCL_L;
		__nop();
		printf("\r\n I2C receive Nack or no Responding ...");
		return 0;
	}
	SCL_L;
	__nop();
	return 1;
}

/*------Slave ACK & NACK Signal--------*/
static void I2C_ACK(void)
{
	SCL_L;
	__nop();
	SDA_L;  // ACK
	__nop();
	SCL_H;
	__nop();
	SCL_L;
	__nop();	
}

static void I2C_NACK(void)
{
	SCL_L;
	SDA_H;  // NACK
	SCL_H;
	SCL_L;
}

/*------Slave Read/Wirte Funcs------*/
// MPU6050 Write Function
static uint8_t Single_WriteI2C(unsigned char Regs_Addr, unsigned char Regs_Data)
{
	if(!I2C_Start())
	{
	 printf("\r\n Warning: I2C start failed ...");
	 return 0;
	}
	I2C_WriteByte(MPU6050Addr);
	if(!I2C_WaitAck())
	{
		I2C_Stop();
		printf("\r\n Warning: Waiting Ack failed ...");
		return 0;
	}
	I2C_WriteByte(Regs_Addr); // Write Address
	I2C_WaitAck();
	I2C_WriteByte(Regs_Data); // Write Data
	I2C_WaitAck();
	I2C_Stop();
	return 1;		
}

static uint8_t Single_ReadI2C(unsigned char Regs_Addr)
{
	uint8_t ret;
	if(!I2C_Start())
	{
	 printf("\r\n Warning: I2C start failed ...");
	 return 0;
	}
	I2C_WriteByte(MPU6050Addr);
	if(!I2C_WaitAck())
	{
		I2C_Stop();
		printf("\r\n Warning: Waiting Ack failed ...");
		return 0;
	}
	I2C_WriteByte(Regs_Addr); // Write Address
	I2C_WaitAck();
	
	// Restart I2C to change to Reading Mode
	I2C_Start();
	__nop();	
	I2C_WriteByte(MPU6050Addr+1); // The LSB bit set 1 to change the mode to Read
	I2C_WaitAck();
	ret = I2C_ReadByte();
	__nop();
	I2C_NACK();
	I2C_Stop();
	return ret;
}

//////////////////////////////////////////////
/*---------MPU6050 Initialisation-----------*/
void I2C2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = Debug1_Pin;// | Debug2_Pin | Debug3_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SCL_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	printf("\r\n I2C	Initialisation finished ...");
}

uint8_t InitMPU6050(void)
{
	I2C2_GPIO_Config();
	delay(0xFFF);
	uint8_t test;
	if(Single_ReadI2C(WHO_AM_I) != 0x68) 
	{
	 test = Single_ReadI2C(WHO_AM_I);
	 printf("\r\n WHO_AM_I = %d",test);
	 printf(",MPU6050 open failed ... \r\n");
	 return 0;
	}
		
	Single_WriteI2C(PWR_MGMT_1, 0x00); //Power Manegement, typical Value: 0x00
	delay(20000);
	
	Single_WriteI2C(SMPLRT_DIV, 0x00); //Gyroscope sampling rate, typical Value: 0x00
	delay(20000);
	
	Single_WriteI2C(CONFIG2, 0x00); //The frequence of the Low-pass Filter, typical Value: 0x00
	delay(20000);
	
	Single_WriteI2C(GYRO_CONFIG, 0x18); //Gyroscope Self-test and Measurement range, typical Value: 0x18
	delay(20000);
	
	Single_WriteI2C(ACCEL_CONFIG, 0x1F); //Acceleration scope Self-test, Measurement range and the frequence of the high pass filter, typical Value: 0x1F
	delay(20000);
	
	printf("\r\n MPU6050	Initialisation Successful ...");
	
	return 1;
}

uint8_t MPU6050_SequenceRead(void)
{
	uint8_t index = 14;
	
	if(!I2C_Start())
	{
	 printf("\r\n Warning: I2C start failed ...");
	 return 0;
	}
	
	I2C_WriteByte(MPU6050Addr);
	if(!I2C_WaitAck())
	{
		I2C_Stop();
		return 0;
	}
	I2C_WriteByte(ACCEL_XOUT_H); // Write Address
	I2C_WaitAck();
	
	// Restart I2C to change to Reading Mode
	I2C_Start(); 							
	I2C_WriteByte(MPU6050Addr+1); // The LSB bit set 1 to change the mode to Read
	I2C_WaitAck();
	while(--index)
	{
		MPU6050_Buffer[13 - index] = I2C_ReadByte();
		I2C_ACK();
	}
	MPU6050_Buffer[13] = I2C_ReadByte();
	I2C_NACK();
	I2C_Stop();
	return 1;
}

void MPU6050_Compose(void)
{
	 uint8_t i;
	 for(i=0; i<8; i++)
	 {
			MPU6050_Data[i] = MPU6050_Buffer[2*i];
			MPU6050_Data[i] <<= 8;
			MPU6050_Data[i] += MPU6050_Buffer[2*i + 1];			
	 }
}

void MPU6050_Print_USART(void)
{
	printf("\r\n ---- GET MPU6050 DATA ---- \r\n");
	// Accelerations 
	printf("\r\n ACCEL_XOUT = %d", MPU6050_Data[0]);
	printf("\r\n ACCEL_YOUT = %d", MPU6050_Data[1]);
	printf("\r\n ACCEL_ZOUT = %d", MPU6050_Data[2]);
	
	// Temperature
	printf("\r\n TEMP_OUT = %d", MPU6050_Data[3]);
	
	// Gyroscopes
	printf("\r\n GYRO_XOUT = %d", MPU6050_Data[4]);
	printf("\r\n GYRO_YOUT = %d", MPU6050_Data[5]);
	printf("\r\n GYRO_ZOUT = %d", MPU6050_Data[6]);
}


