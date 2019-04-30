#ifndef __PID_H
#define __PID_H

typedef struct
{
	float Kp;  	//比例系数Proportional
	float Ki; 		//积分系数Integral
	float Kd; 		//微分系数Derivative
	
	float Ek; 		//当前误差
	float Ek1;		//前一次误差 e(k-1)
	float Ek2;		//再前一次误差 e(k-2)
	float LocSum;  //累计积分位置
}PID_LocTypeDef;


float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID);

#endif
