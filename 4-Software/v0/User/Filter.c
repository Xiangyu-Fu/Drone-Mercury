#include "math.h"
#include "Maths.h"
#include "Filter.h"
#include "struct.h"

static float ACC_IIR_FACTOR;

 /**
  * @brief  calculate the parameter of the IIR low pass filter
  * @param  Time: the time of response
  * @param  Cut_Off: ?
  * @retval void
  */
void Calculate_FilteringCoefficient(float Time, float Cut_Off)
{
	ACC_IIR_FACTOR = Time /( Time + 1/(2.0f*Pi*Cut_Off) );
}


 /**
  * @brief  the IIR low pass filter of the acceleration 
  * @param  void
  * @retval void
  */
void ACC_IIR_Filter(struct _acc *Acc_in,struct _acc *Acc_out)
{
	Acc_out->x = Acc_out->x + ACC_IIR_FACTOR*(Acc_in->x - Acc_out->x); 
	Acc_out->y = Acc_out->y + ACC_IIR_FACTOR*(Acc_in->y - Acc_out->y); 
	Acc_out->z = Acc_out->z + ACC_IIR_FACTOR*(Acc_in->z - Acc_out->z); 
}

#define Filter_Num 2


 /**
  * @brief  Gyro window sliding filter
  * @param  _gyro struct gyro raw input
  * @retval _gyro struct gyro filtered output
  */
void Gyro_Filter(struct _gyro *Gyro_in,struct _gyro *Gyro_out)
{
	static int16_t Filter_x[Filter_Num],Filter_y[Filter_Num],Filter_z[Filter_Num];  // buffer definition
	static uint8_t Filter_count;                                    								// counter definition
	int32_t Filter_sum_x=0,Filter_sum_y=0,Filter_sum_z=0;														// sum definition
	uint8_t i=0;
	
	Filter_x[Filter_count] = Gyro_in->x;
	Filter_y[Filter_count] = Gyro_in->y;
	Filter_z[Filter_count] = Gyro_in->z;

	// calculate the sum 
	for(i=0;i<Filter_Num;i++)
	{
		Filter_sum_x += Filter_x[i];
		Filter_sum_y += Filter_y[i];
		Filter_sum_z += Filter_z[i];
	}	
	
	Gyro_out->x = Filter_sum_x / Filter_Num;
	Gyro_out->y = Filter_sum_y / Filter_Num;
	Gyro_out->z = Filter_sum_z / Filter_Num;
	
	Filter_count++;
	if(Filter_count == Filter_Num)
		Filter_count=0;
}
