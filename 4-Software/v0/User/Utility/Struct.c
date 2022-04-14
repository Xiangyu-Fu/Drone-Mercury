#include "struct.h"
#include "EEPROM.h"
#include "Filter.h"
#include "Struct.h"

// raw data
struct _acc  acc;						
struct _gyro gyro;

// filtered data
struct _acc  filter_acc;		
struct _gyro filter_gyro;

// zero-offset data
struct _acc  offset_acc;		
struct _gyro offset_gyro;


struct _SI_float  SI_acc;		// acceleration data struct��m/s2��
struct _SI_float  SI_gyro;	// angular velocity datat struct��rad��


struct _Rc Rc;							// remote control data
struct _out_angle out_angle;// Attitute Solver - Angle Value

/* pid */
struct _pid pitch;
struct _pid roll;
struct _pid gyro_pitch;
struct _pid gyro_roll;
struct _pid gyro_yaw;


uint16_t data_x,data_y,data_z;
uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06,0x9A07,0x9A08,0x9A09,
	0x9A10,0x9A12,0x9A13,0x9A14,0x9A15
};//�����ַ

 /**
  * @brief  Reset PID
  * @param  void
  * @retval void
  */
void PID_Reset(void)
{
	LED3_ON;
//	roll.kp  = 10.0f;
//	roll.ki  = 0.07f;
//	roll.kd  = 10.0f;
//	pitch.kp = 10.0f;
//	pitch.ki = 0.07f;
//	pitch.kd = 10.0f;
//	gyro_roll.kp  =1.7f;
//	gyro_roll.ki  = 0.0f;
//	gyro_roll.kd  =18.0f;
//	gyro_pitch.kp =1.7f;
//	gyro_pitch.ki = 0.0f;
//	gyro_pitch.kd =18.0f;
//	gyro_yaw.kp   = 8.0f;
//	gyro_yaw.ki   = 0.1f;
//	gyro_yaw.kd   = 0.1f;
	roll.kp  = 4.9f;
	roll.ki  = 0.02f;
	roll.kd  = 8.0f;
	pitch.kp = 4.9f;
	pitch.ki = 0.02f;
	pitch.kd = 8.0f;
	gyro_roll.kp  =1.4f;
	gyro_roll.ki  = 0.0f;
	gyro_roll.kd  =2.0f;
	gyro_pitch.kp =1.4f;
	gyro_pitch.ki = 0.0f;
	gyro_pitch.kd =2.0f;
	gyro_yaw.kp   = 8.0f;
	gyro_yaw.ki   = 0.1f;
	gyro_yaw.kd   = 0.1f;

	EEPROM_SAVE_PID();
}
///////////////////////////////////////////////////////////
#define EEPROM_offset_acc_x		0
#define EEPROM_offset_acc_y		1
#define EEPROM_offset_acc_z		2
#define EEPROM_offset_gyro_x	3
#define EEPROM_offset_gyro_y	4
#define EEPROM_offset_gyro_z	5
#define EEPROM_PID_ROL_P		6
#define EEPROM_PID_ROL_I		7
#define EEPROM_PID_ROL_D		8
#define EEPROM_PID_PIT_P		9
#define EEPROM_PID_PIT_I		10
#define EEPROM_PID_PIT_D		11
#define EEPROM_PID_YAW_P		12
#define EEPROM_PID_YAW_I		13
#define EEPROM_PID_YAW_D		14
#define EEPROM_SUM				15


 /**
  * @brief  Reset PID
  * @param  void
  * @retval void
  */
void EEPROM_INIT(void)//��ʼ��ģ���EEPROM����ȡ����
{
	EE_Init();	
	EEPROM_READ_ACC_OFFSET();
	EEPROM_READ_GYRO_OFFSET();
	EEPROM_READ_PID();
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_SUM], &data_x);
	if(data_x != EEPROM_SUM)//����Ƿ��״��Է�
	{
		__set_PRIMASK(1);//��ȫ���ж�
		EE_WriteVariable(VirtAddVarTab[EEPROM_SUM], EEPROM_SUM);
		 __set_PRIMASK(0);//���ж�
		PID_Reset();//�״��Էɣ�����PID����ΪĬ��ֵ
	}
}

void EEPROM_SAVE_ACC_OFFSET(void)	//����acc��ƫ
{
	__set_PRIMASK(1);//��ȫ���ж�
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_x], offset_acc.x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_y], offset_acc.y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_z], offset_acc.z);
	 __set_PRIMASK(0);//���ж�
}

void EEPROM_READ_ACC_OFFSET(void)	//��ȡacc��ƫ
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_x], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_y], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_z], &data_z);
	offset_acc.x = (int16_t) data_x;
	offset_acc.y = (int16_t) data_y;
	offset_acc.z = (int16_t) data_z;
}

void EEPROM_SAVE_GYRO_OFFSET(void)	//����gyro��ƫ
{
	 __set_PRIMASK(1);//��ȫ���ж�
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_x], offset_gyro.x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_y], offset_gyro.y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_z], offset_gyro.z);
	 __set_PRIMASK(0);//���ж�
}

void EEPROM_READ_GYRO_OFFSET(void)	//��ȡgyro��ƫ
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_x], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_y], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_z], &data_z);
	offset_gyro.x = (int16_t) data_x;
	offset_gyro.y = (int16_t) data_y;
	offset_gyro.z = (int16_t) data_z;
}

void EEPROM_SAVE_PID(void)	//����PID����
{
	 __set_PRIMASK(1);//��ȫ���ж�
	data_x = (int16_t) (roll.kp*10);
	data_y = (int16_t) (roll.ki*1000);
	data_z = (int16_t) (roll.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_D], data_z);

	data_x = (int16_t) (gyro_roll.kp*10);
	data_y = (int16_t) (gyro_roll.ki*1000);
	data_z = (int16_t) (gyro_roll.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_D], data_z);

	data_x = (int16_t) (gyro_yaw.kp*10);
	data_y = (int16_t) (gyro_yaw.ki*1000);
	data_z = (int16_t) (gyro_yaw.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_D], data_z);
	__set_PRIMASK(0);//���ж�
}

void EEPROM_READ_PID(void)	// ��ȡPID����
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_D], &data_z);
	roll.kp = (float) data_x/10.0f;
	roll.ki = (float) data_y/1000.0f;
	roll.kd = (float) data_z;
	pitch.kp = roll.kp;
	pitch.ki = roll.ki;
	pitch.kd = roll.kd;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_D], &data_z);
	gyro_roll.kp = (float) data_x/10.0f;
	gyro_roll.ki = (float) data_y/1000.0f;
	gyro_roll.kd = (float) data_z;
	gyro_pitch.kp = gyro_roll.kp;
	gyro_pitch.ki = gyro_roll.ki;
	gyro_pitch.kd = gyro_roll.kd;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_D], &data_z);
	gyro_yaw.kp = (float) data_x/10.0f;
	gyro_yaw.ki = (float) data_y/1000.0f;
	gyro_yaw.kd = (float) data_z;
}
