#ifndef __PID_H
#define __PID_H

typedef struct
{
	float Kp;  	//����ϵ��Proportional
	float Ki; 		//����ϵ��Integral
	float Kd; 		//΢��ϵ��Derivative
	
	float Ek; 		//��ǰ���
	float Ek1;		//ǰһ����� e(k-1)
	float Ek2;		//��ǰһ����� e(k-2)
	float LocSum;  //�ۼƻ���λ��
}PID_LocTypeDef;


float PID_Loc(float SetValue, float ActualValue, PID_LocTypeDef *PID);

#endif
