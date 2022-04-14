#include "IMU.h"
#include "math.h"
#include "Maths.h"

// global parameters definitions
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     	// Quaternion
float exInt = 0, eyInt = 0, ezInt = 0;    	// Error integral accumulated value


 /**
  * @brief  attitude solver and update
  * @param  gx,gy,gz: the gyro data of three axis
	* @param  ax,ay,az: the accerlation data of three axis
  * @retval void
  */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;

	// integration of quaternion, and get the current attitude
	float q0_last = q0;	
	float q1_last = q1;	
	float q2_last = q2;	
	float q3_last = q3;

	// translate the three dimentional vector of the accelerometer to the unit vector
	norm = invSqrt(ax*ax + ay*ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	// estimate the expression of the gravity acceleration in the coordinate system of the drone, 
  // which is the third line of the rotaion matrix expressed by quaternion
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	// estimate the error between the direction measured by accelerometer and the direction of the gravity acceleration, 
	// calculated by the cross product of vector
	ex = ay * vz - az * vy;
	ey = az*vx - ax*vz;
	ez = ax*vy - ay*vx;

	// accumulated error, multipling with intergual constant
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

	// PI gyro zero offset correction using cross product, i.e. offsetting the offset from the gyro reading 
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;


	// first order approximate algorithm
	q0 = q0_last + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
	q1 = q1_last + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
	q2 = q2_last + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
	q3 = q3_last + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT; 

	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;

	out_angle.yaw  +=  filter_gyro.z * RawData_to_Angle * 0.001f;
}

 /**
  * @brief  Gyro window sliding filter
  * @param  _gyro struct gyro raw input
  * @retval _gyro struct gyro filtered output
  */
void Get_Eulerian_Angle(struct _out_angle *angle)
{
	angle->pitch = -atan2(2.0f*(q0*q1 + q2*q3),q0*q0 - q1*q1 - q2*q2 + q3*q3)*Radian_to_Angle;
	angle->roll  =  asin (2.0f*(q0*q2 - q1*q3))*Radian_to_Angle;
}
