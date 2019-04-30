#include "pid.h"



/************************************************
函数名称 ： PID_Loc
功	  能 ： PID位置(Location)计算
参	  数 ： SetValue ------ 设置值(期望值)
			ActualValue --- 实际值(反馈值)
			PID ----------- PID数据结构
返 回 值 ： PIDLoc -------- PID位置
作 	  者 ： strongerHuang
*************************************************/
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
	float PIDLoc; 		//位置
	
	PID->Ek = SetValue - ActualValue;
	PID->LocSum += PID->Ek; 	 //累计误差
	
	PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek1 - PID->Ek);
	
	PID->Ek1 = PID->Ek; 
	return PIDLoc;
}