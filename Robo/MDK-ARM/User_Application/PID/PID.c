///////////////////////////////////////
//	***	*** 
//	 *  **
//	***	*
//  write by Ideal_Fox
//  Affiliated to Suzhou City University
//  Revision made on July 13th, 2026
///////////////////////////////////////
//	文件介绍：
//		该文件为PID算法源文件
///////////////////////////////////////
#include "PID.h"
#include "stm32f4xx_hal.h"

#include "Encoder.h"
#include "Motor.h"

#include "Control.h"

//电机PID控制结构体
PID_Incremental PID_Motor1;
PID_Incremental PID_Motor2;
PID_Incremental PID_Motor3;
PID_Incremental PID_Motor4;

/** @brief	PID参数初始化
  **/
void PID_Parameter_Init(void)
{
	//内环，电机速度控制，增量式PID
	//M1
	PID_Motor1.Kp 		= 43;		//比例系数
	PID_Motor1.Ki 		= 0.8f;		//积分系数
	PID_Motor1.Kd 		= 9;		//微分系数
	PID_Motor1.Out_Max	= 60;		//最大限幅
	PID_Motor1.Out_Min	= 0;		//最小限幅
	PID_Motor1.DT		= 5;		//调控周期，单位ms
	//M2
	PID_Motor2.Kp 		= 43;		//比例系数
	PID_Motor2.Ki 		= 0.8f;		//积分系数
	PID_Motor2.Kd 		= 9;		//微分系数
	PID_Motor2.Out_Max	= 60;		//最大限幅
	PID_Motor2.Out_Min	= 0;		//最小限幅
	PID_Motor2.DT		= 5;		//调控周期，单位ms
	//M3
	PID_Motor3.Kp 		= 43;		//比例系数
	PID_Motor3.Ki 		= 0.8f;		//积分系数
	PID_Motor3.Kd 		= 9;		//微分系数
	PID_Motor3.Out_Max	= 60;		//最大限幅
	PID_Motor3.Out_Min	= 0;		//最小限幅
	PID_Motor3.DT		= 5;		//调控周期，单位ms
	//M4
	PID_Motor4.Kp 		= 43;		//比例系数
	PID_Motor4.Ki 		= 0.8f;		//积分系数
	PID_Motor4.Kd 		= 9;		//微分系数
	PID_Motor4.Out_Max	= 60;		//最大限幅
	PID_Motor4.Out_Min	= 0;		//最小限幅
	PID_Motor4.DT		= 5;		//调控周期，单位ms
}


/** @brief	电机速度控制
  * @note	内环PID解算，输出控制
  **/
void Motor_Speed_Control(void)
{
	//传递实际值，传递参数为电机实际转速
	//M1实际值
	PID_Motor1.Actual_Last_Last	= PID_Motor1.Actual_Last;
	PID_Motor1.Actual_Last 		= PID_Motor1.Actual;
	PID_Motor1.Actual 			= Encoder1.RMP_S;
	
	//M2实际值
	PID_Motor2.Actual_Last_Last	= PID_Motor2.Actual_Last;
	PID_Motor2.Actual_Last 		= PID_Motor2.Actual;
	PID_Motor2.Actual 			= Encoder2.RMP_S;
	
	//M3实际值
	PID_Motor3.Actual_Last_Last	= PID_Motor3.Actual_Last;
	PID_Motor3.Actual_Last 		= PID_Motor3.Actual;
	PID_Motor3.Actual 			= Encoder3.RMP_S;
	
	//M4实际值
	PID_Motor4.Actual_Last_Last	= PID_Motor4.Actual_Last;
	PID_Motor4.Actual_Last 		= PID_Motor4.Actual;
	PID_Motor4.Actual 			= Encoder4.RMP_S;
	
	//传递调控值，传递参数为电机目标转速
	PID_Motor1.Target = Control_Parm.Motor1_Speed;
	PID_Motor2.Target = Control_Parm.Motor2_Speed;
	PID_Motor3.Target = Control_Parm.Motor3_Speed;
	PID_Motor4.Target = Control_Parm.Motor4_Speed;
	
	//进行PID计算，输出参数为PWM占空比
	PID_Incremental_Compute(&PID_Motor1);
	PID_Incremental_Compute(&PID_Motor2);
	PID_Incremental_Compute(&PID_Motor3);
	PID_Incremental_Compute(&PID_Motor4);
	
	//执行控制
	Motor_Control_One(M1,Control_Parm.M1,PID_Motor1.Out);
	Motor_Control_One(M2,Control_Parm.M2,PID_Motor2.Out);
	Motor_Control_One(M3,Control_Parm.M3,PID_Motor3.Out);
	Motor_Control_One(M4,Control_Parm.M4,PID_Motor4.Out);
}

/** @brief	增量式PID运算函数
  * @param	Pid		PID结构体
  * @note	其包含二阶微分先行，调控周期自适应
  **/
void PID_Incremental_Compute(PID_Incremental* Pid)
{
	//获取偏差
	Pid->Error_Last	= Pid->Error_Now;				//计算上次偏差
	Pid->Error_Now	= Pid->Target - Pid->Actual;	//计算当前偏差
	
	//PID计算		
	//Kp_Out = Kp*(当前偏差-上次偏差)
	Pid->Kp_Out = Pid->Kp * (Pid->Error_Now - Pid->Error_Last);
	
	//Ki_Out = Ki*当前偏差*DT
	Pid->Ki_Out = (Pid->Ki *  Pid->Error_Now) * Pid->DT;
	
	//Kd_Out = Kd*(当前反馈 - 2倍上次反馈 + 上上次反馈)/DT
	Pid->Kd_Out = Pid->Kd * (Pid->Actual - 2*Pid->Actual_Last + Pid->Actual_Last_Last) / Pid->DT;
	
	//输出计算
	Pid->Out += (Pid->Kp_Out + Pid->Ki_Out - Pid->Kd_Out);
	
	//输出死区控制，目标值为0，则关断输出
	if(Pid->Target == 0) Pid->Out = 0;
	
	//输出限幅
	if(Pid->Out > Pid->Out_Max)	Pid->Out = Pid->Out_Max;
	if(Pid->Out < Pid->Out_Min)	Pid->Out = Pid->Out_Min;
}

