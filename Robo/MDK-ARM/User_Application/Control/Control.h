///////////////////////////////////////
//	***	*** 
//	 *  **
//	***	*
//  write by Ideal_Fox
//  Affiliated to Suzhou City University
//  Revision made on July 13th, 2026
///////////////////////////////////////
//	文件介绍：
//		该文件为控制输出头文件
///////////////////////////////////////
#ifndef __Control_H__
#define __Control_H__

#include "Motor.h"

//控制状态结构体
struct Control{
	Motor_State M1;		//电机1运行方向
	Motor_State M2;		//电机2运行方向
	Motor_State M3;		//电机3运行方向
	Motor_State M4;		//电机4运行方向
	
	float Motor1_Speed;	//电机1目标速度，单位rmp/s
	float Motor2_Speed;	//电机2目标速度，单位rmp/s
	float Motor3_Speed;	//电机3目标速度，单位rmp/s
	float Motor4_Speed;	//电机4目标速度，单位rmp/s
	
	char Grayscale_Calibration_State;	//灰度传感器校准状态	0：无校准，1
};
extern struct Control Control_Parm;





#endif
