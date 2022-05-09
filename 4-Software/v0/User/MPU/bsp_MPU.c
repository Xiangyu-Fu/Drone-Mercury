#include "stdio.h"
#include "bsp_log.h"
#include "bsp_MPU.h"
#include "utility.h"


/******************************************************************************
							宏定义
*******************************************************************************/ 
#define SCL_Pin 	GPIO_Pin_10
#define SDA_Pin 	GPIO_Pin_11

#define	SCL_H   	{GPIOB->BSRR = SCL_Pin;I2C_delay(4);}	 //SCL高电平
#define	SCL_L   	{GPIOB->BRR  = SCL_Pin;I2C_delay(4);}	 //SCL低电平
#define	SDA_H   	{GPIOB->BSRR = SDA_Pin;I2C_delay(4);}	 //SDA高电平
#define	SDA_L   	{GPIOB->BRR  = SDA_Pin;I2C_delay(4);}	 //SDA低电平
#define	SDA_Read	GPIOB->IDR  & SDA_Pin	 //SDA读数据

/******************************************************************************
							全局变量
*******************************************************************************/ 
uint8_t	GYRO_Offset = 0;//不自动校正
uint8_t	ACC_Offset  = 0;
uint32_t I2C_Erro=0;
/******************************************************************************
							局部变量
*******************************************************************************/ 
static uint8_t	MPU6050_Buffer[14];	//I2C读取数据缓存


uint32_t MPU6050_Buffer[14];
uint32_t MPU6050_Data[7];

static const char *TAG = "MPU6050";

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
		STM_LOGW(TAG,"I2C receive Nack or no Responding");
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
	 STM_LOGW(TAG,"Warning: I2C start failed");
	 return 0;
	}
	I2C_WriteByte(MPU6050Addr);
	if(!I2C_WaitAck())
	{
		I2C_Stop();
		STM_LOGW(TAG,"Warning: Waiting Ack failed");
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
		STM_LOGW(TAG,"Warning: Waiting Ack failed ...");
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

	GPIO_InitStructure.GPIO_Pin = Debug1_Pin| Debug2_Pin | Debug3_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SCL_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	STM_LOGI(TAG,"I2C Initialisation finished ...");
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
	delay(2000);
	
	Single_WriteI2C(SMPLRT_DIV, 0x00); //Gyroscope sampling rate, typical Value: 0x00
	delay(2000);
	
	Single_WriteI2C(CONFIG2, 0x00); //The frequence of the Low-pass Filter, typical Value: 0x00
	delay(2000);
	
	Single_WriteI2C(GYRO_CONFIG, 0x18); //Gyroscope Self-test and Measurement range, typical Value: 0x18
	delay(2000);
	
	Single_WriteI2C(ACCEL_CONFIG, 0x1F); //Acceleration scope Self-test, Measurement range and the frequence of the high pass filter, typical Value: 0x1F
	delay(2000);
	
	STM_LOGI(TAG, "MPU6050 Initialisation Successful");
	
	return 1;
}

uint8_t MPU6050_SequenceRead(void)
{
	uint8_t index = 14;
	
	if(!I2C_Start())
	{
	 STM_LOGW(TAG, "Warning: I2C start failed");
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


/******************************************************************************
函数原型：	static uint8_t Single_ReadI2C(unsigned char Regs_Addr) 	
功    能：	读MPU6050单字节
返 回 值：	寄存器值
*******************************************************************************/ 
static uint8_t Single_ReadI2C(unsigned char Regs_Addr) 
{
	uint8_t ret;

	if( !I2C_Start() )
		return 0;	//I2C起始错误，返回
	I2C_WriteByte(MPU6050Address); 	//写Slave地址，并配置成写模式
	if( !I2C_WaitAck() )
	{
		I2C_Stop(); 
		return 0;	//无ACK，返回
	}
	
	I2C_WriteByte(Regs_Addr);    	//写寄存器地址
	I2C_WaitAck();
	I2C_Start();
	I2C_WriteByte(MPU6050Address+1);//写Slave地址，并配置成读模式
	I2C_WaitAck();
	
	ret=I2C_ReadByte();				//从传感器中读出数据

	I2C_NACK();						//无应答
	I2C_Stop();                  	//结束本段IIC进程 
	return ret;	
} 

/******************************************************************************
函数原型：	uint8_t MPU6050_SequenceRead(void) 	
功    能：	连续读MPU6050数据寄存器
返 回 值：	0出错；1成功
*******************************************************************************/ 
uint8_t MPU6050_SequenceRead(void)
{
	uint8_t index=14;
	
	if( !I2C_Start() )
		return 0;	//I2C起始错误，返回
	I2C_WriteByte(MPU6050Address); 	//写Slave地址，并配置成写模式
	if( !I2C_WaitAck() )
	{
		I2C_Stop(); 
		return 0;	//无ACK，返回
	}
	I2C_WriteByte(ACCEL_XOUT_H);    //写寄存器地址
	I2C_WaitAck();
	
	I2C_Start();
	I2C_WriteByte(MPU6050Address+1);//写Slave地址，并配置成读模式
	I2C_WaitAck();

	while(--index)	//连读13位寄存器
	{
		MPU6050_Buffer[13 - index] = I2C_ReadByte();
		I2C_ACK();
	}

	MPU6050_Buffer[13] = I2C_ReadByte();	//读第14寄存器
	I2C_NACK();	
	I2C_Stop();	
	
	return 1;
}
/******************************************************************************
函数原型：	void MPU6050_SingleRead(void)
功    能：	分次读MPU6050数据寄存器
*******************************************************************************/ 
void MPU6050_SingleRead(void)
{
		MPU6050_Buffer[0] = Single_ReadI2C(ACCEL_XOUT_H);
		MPU6050_Buffer[1] = Single_ReadI2C(ACCEL_XOUT_L);		
		MPU6050_Buffer[2] = Single_ReadI2C(ACCEL_YOUT_H);	
		MPU6050_Buffer[3] = Single_ReadI2C(ACCEL_YOUT_L);
		MPU6050_Buffer[4] = Single_ReadI2C(ACCEL_ZOUT_H);
		MPU6050_Buffer[5] = Single_ReadI2C(ACCEL_ZOUT_L);
		
		MPU6050_Buffer[8]  = Single_ReadI2C(GYRO_XOUT_H);
		MPU6050_Buffer[9]  = Single_ReadI2C(GYRO_XOUT_L);
		MPU6050_Buffer[10] = Single_ReadI2C(GYRO_YOUT_H);
		MPU6050_Buffer[11] = Single_ReadI2C(GYRO_YOUT_L);
		MPU6050_Buffer[12] = Single_ReadI2C(GYRO_ZOUT_H);
		MPU6050_Buffer[13] = Single_ReadI2C(GYRO_ZOUT_L);
}
/******************************************************************************
函数原型：	uint8_t MPU6050_SingleRead(void)
功    能：	初始化MPU6050
返 回 值：	0出错；1成功
*******************************************************************************/ 
uint8_t InitMPU6050(void)
{
	if( Single_ReadI2C(WHO_AM_I) != 0x68)//检查MPU6050是否正常
	{
		return 0;
	}
	
	PrintString("\r\n MPU6050  正常！");
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//电源管理，典型值：0x00，正常模式
	I2C_delay(20000); //约2.5ms延时
	Single_WriteI2C(SMPLRT_DIV, 0x00);	//陀螺仪采样率，典型值：0x00，不分频（8KHZ）
	I2C_delay(20000);            
	Single_WriteI2C(CONFIG2, 0x00);   	//低通滤波频率，典型值：0x00，不启用MPU6050自带滤波
	I2C_delay(20000);
	Single_WriteI2C(GYRO_CONFIG, 0x18);	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C_delay(20000);
	Single_WriteI2C(ACCEL_CONFIG, 0x1F);//加速计自检、测量范围及高通滤波频率，典型值：0x1F(不自检，16G)
	I2C_delay(20000);
	
	PrintString("\r\n MPU6050  初始化完成！");
	return 1;
}
/******************************************************************************
函数原型：	void Do_ACC_Offset(void)
功    能：	MPU6050加速度零偏校正
*******************************************************************************/ 
void Do_ACC_Offset(void)
{
	ACC_Offset=1;
}

/******************************************************************************
函数原型：	void Do_GYRO_Offset(void)
功    能：	MPU6050角速度零偏校正
*******************************************************************************/ 
void Do_GYRO_Offset(void)
{
	GYRO_Offset=1;
}

/******************************************************************************
函数原型：	void MPU6050_Offset(void)
功    能：	MPU6050零偏校正
*******************************************************************************/ 
void MPU6050_Offset(void)
{
	if(ACC_Offset)
	{
		static int32_t ACC_X=0,ACC_Y=0,ACC_Z=0;
		static uint8_t count_acc=0;
		if(count_acc==0)
		{
			offset_acc.x = 0;
			offset_acc.y = 0;
			offset_acc.z = 0;
			ACC_X = 0;
			ACC_Y = 0;
			ACC_Z = 0;
			count_acc = 1;
			return;
		}
		else
		{
			count_acc++;
			ACC_X += acc.x;
			ACC_Y += acc.y;
			ACC_Z += acc.z;
		}
		if(count_acc==251)
		{
			count_acc--;
			offset_acc.x = ACC_X / count_acc;
			offset_acc.y = ACC_Y / count_acc;
			offset_acc.z = ACC_Z / count_acc - 2048;
			count_acc = 0;
			ACC_Offset = 0;
			EEPROM_SAVE_ACC_OFFSET();
			
		}
		LED3_ON;
	}
	
	if(GYRO_Offset)
	{
		static int32_t GYRO_X=0,GYRO_Y=0,GYRO_Z=0;
		static uint8_t count_gyro=0;
		if(count_gyro==0)
		{
			offset_gyro.x = 0;
			offset_gyro.y  = 0;
			offset_gyro.z   = 0;
			GYRO_X = 0;
			GYRO_Y = 0;
			GYRO_Z = 0;
			count_gyro = 1;
			return;
		}
		else
		{
			count_gyro++;
			GYRO_X += gyro.x;
			GYRO_Y += gyro.y;
			GYRO_Z += gyro.z;
		}
		if(count_gyro==251)
		{
			count_gyro--;
			offset_gyro.x = GYRO_X / count_gyro;
			offset_gyro.y = GYRO_Y / count_gyro;
			offset_gyro.z = GYRO_Z / count_gyro;
			count_gyro = 0;
			GYRO_Offset = 0;
			EEPROM_SAVE_GYRO_OFFSET();
		}
		LED3_ON;
	}
}

/******************************************************************************
函数原型：	void MPU6050_Compose(void)
功    能：	合成MPU6050的16位数据
*******************************************************************************/ 
void MPU6050_Compose(void)
{
	acc.x  = ((((int16_t)MPU6050_Buffer[0]) << 8) | MPU6050_Buffer[1]) - offset_acc.x;	//减去零偏
	acc.y  = ((((int16_t)MPU6050_Buffer[2]) << 8) | MPU6050_Buffer[3]) - offset_acc.y;
	acc.z  = ((((int16_t)MPU6050_Buffer[4]) << 8) | MPU6050_Buffer[5]) - offset_acc.z;

	gyro.x = ((((int16_t)MPU6050_Buffer[8])  << 8) | MPU6050_Buffer[9])  - offset_gyro.x;
	gyro.y = ((((int16_t)MPU6050_Buffer[10]) << 8) | MPU6050_Buffer[11]) - offset_gyro.y;
	gyro.z = ((((int16_t)MPU6050_Buffer[12]) << 8) | MPU6050_Buffer[13]) - offset_gyro.z;
	
	MPU6050_Offset();
}

/******************************************************************************
函数原型：	void MPU6050_Printf(void)
功    能：	输出MPU6050的原始数据
*******************************************************************************/ 
void MPU6050_Printf(void)
{
	PrintString("\r\nMPU6050 acc:\r\n");
	PrintS16(acc.x);
	PrintS16(acc.y);
	PrintS16(acc.z);
	PrintString("\r\nMPU6050 gyro\r\n");
	PrintS16(gyro.x);
	PrintS16(gyro.y);
	PrintS16(gyro.z);
}



