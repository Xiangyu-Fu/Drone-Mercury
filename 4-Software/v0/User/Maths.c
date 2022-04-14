#include "Maths.h"
#include "struct.h"


 /**
  * @brief  translate the raw data to radian
  * @param  gyro raw input data
  * @retval translated radian data 
  */
void Get_Radian(struct _gyro *Gyro_in, struct _SI_float *Gyro_out)
{
	Gyro_out->x = (float)(Gyro_in->x * RawData_to_Radian);
	Gyro_out->y = (float)(Gyro_in->y * RawData_to_Radian);
	Gyro_out->z = (float)(Gyro_in->z * RawData_to_Radian);
}

 /**
  * @brief  fast calculate the inverse num of the sqrt 
  * @param  parameter x
  * @retval the inverse number of the sqrt
  */
float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y =x;
	long i = *(long *)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float *)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

uint8_t Get_Checksum(uint8_t mydata[])
{	
	uint8_t i;
    uint8_t checksum=0;
	uint8_t length = mydata[3];

    for(i=0;i<length+2;i++)
    {
        checksum ^= (mydata[3+i]&0xFF);
    }
    return checksum;
}