#include "pid.h"



/************************************************
�������� �� PID_Loc
��	  �� �� PIDλ��(Location)����
��	  �� �� SetValue ------ ����ֵ(����ֵ)
			ActualValue --- ʵ��ֵ(����ֵ)
			PID ----------- PID���ݽṹ
�� �� ֵ �� PIDLoc -------- PIDλ��
�� 	  �� �� strongerHuang
*************************************************/
float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID)
{
	float PIDLoc; 		//λ��
	
	PID->Ek = SetValue - ActualValue;
	PID->LocSum += PID->Ek; 	 //�ۼ����
	
	PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek1 - PID->Ek);
	
	PID->Ek1 = PID->Ek; 
	return PIDLoc;
}