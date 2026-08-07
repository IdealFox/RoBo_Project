///////////////////////////////////////
//	***	*** 
//	 *  **
//	***	*
//  write by Ideal_Fox
//  Affiliated to Suzhou City University
//  Revision made on July 15th, 2026
///////////////////////////////////////
//	文件介绍：
//		该文件为设置与显示头文件
///////////////////////////////////////
#ifndef __Set_And_Show_H__
#define __Set_And_Show_H__

#include "KEY.h"
/*
设置/显示菜单定义，文字为菜单名称，数字为菜单界面码

一级菜单+-二级系统设置菜单(0)---+-三级速度设置菜单(00)--+-基础速度设置
		|						|						+-转向速度设置
		|						|						
		|						+-三级PID设置菜单(01)---+-轮子转速环PID
		|												+-巡线调控环PID
		|
		+-二级巡线设置菜单(1)---+-三级灰度参数查看菜单(10)--+-校准值显示
		|						|							+-实际值显示
		|						|							
		|						+-三级校准设置菜单(11)--+-最大极限值校准
		|						|						+-最小极限值校准
		|						|
		|						+-计算模式设置
		|						+-触发阈值设置
		|						+-参数存储
		|						
		+-二级舵机设置菜单(2)---+-三级舵机直接控制菜单(20)--+-舵机选择
		|													+-舵机状态设置
		|													+-舵机方向设置
		|													+-舵机角度设置
		|													+-舵机最大极限角度设置
		|													+-舵机最小极限角度设置
		|													+-参数存储
		|
		+-二级任务设置窗口(3)---+-三级运行指示灯任务查看(30)
								+-三级按键扫描任务查看(31)
								+-三级设置与显示任务查看(32)
								+-三级串口调试任务查看(33)
								+-三级低速计算任务查看(34)
								+-三级高速计算任务查看(35)
*/

//菜单等级枚举
typedef enum{
	Level1 = 0,
	Level2,
	Level3,
	Level4,
	Level5
}Menu_Level_Set;

//二级菜单界面码枚举
typedef enum{
	System_Set   	= 0,	//系统设置菜单
	Scan_Line_Set	= 1,	//巡线参数设置菜单
	Servo_Set		= 2,	//舵机设置菜单
	Task_Show		= 3,	//任务查看菜单	
}Menu_IF_L2;

//三级菜单界面码枚举
typedef enum{
	Speed_Set      = 0,   // 速度设置
	PID_Set        = 1,   // PID设置

	Gray_View      = 10,  // 灰度参数查看
	Gray_Calib     = 11,  // 校准设置

	Servo_Set_	   = 20,  // 舵机控制

	Task_LED       = 30,  // 运行指示灯任务
	Task_KEY_Sacn  = 31,  // 按键扫描任务
	Task_Show_Set  = 32,  // 设置与显示任务
	Task_UART_Debug= 33,  // 串口调试任务
	Task_Slow_Com   = 34,  // 低速计算任务
	Task_High_Com  = 35,  // 高速计算任务	
}Menu_IF_L3;

//系统菜单结构体
struct Menu{
	Menu_Level_Set Menu_Level;		//菜单当前等级
	int Menu_Control_Pointer;		//菜单控制指针
	int Menu_Interface_L2;			//二级菜单界面码
	int Menu_Interface_L3;			//三级菜单界面吗
};
extern struct Menu Menu_Parm;


void System_Menu_Control_And_Show(void);		//系统菜单控制与显示

//一级主菜单控制与显示
void Menu_Level1_Control(KEY_Tigger_State KEY1,KEY_Tigger_State Rocker_UP,KEY_Tigger_State Rocker_Down);

//二级系统菜单控制与显示
void Menu_Level2_System_Set(KEY_Tigger_State KEY1,KEY_Tigger_State KEY2,KEY_Tigger_State Rocker_UP,KEY_Tigger_State Rocker_Down);
//二级巡线菜单控制与显示
void Menu_Level2_Scan_Line_Set(KEY_Tigger_State KEY1,KEY_Tigger_State KEY2,KEY_Tigger_State Rocker_UP,KEY_Tigger_State Rocker_Down);
//二级舵机设置菜单控制与显示
void Menu_Level2_Servo_Set(KEY_Tigger_State KEY1,KEY_Tigger_State KEY2);
//二级任务设置窗口控制与显示
void Menu_Level2_Task_Show(KEY_Tigger_State KEY1,KEY_Tigger_State KEY2,KEY_Tigger_State Rocker_UP,KEY_Tigger_State Rocker_Down);

//三级速度设置菜单控制与显示
void Menu_Level3_Speed_Set(KEY_Tigger_State KEY2,KEY_Tigger_State Rocker_UP,KEY_Tigger_State Rocker_Down);
//三级PID设置菜单控制与显示
void Menu_Level3_PID_Set(KEY_Tigger_State KEY2,KEY_Tigger_State Rocker_UP,KEY_Tigger_State Rocker_Down);
//三级灰度参数查看控制与显示
void Menu_Level3_Gray_View(KEY_Tigger_State KEY2,KEY_Tigger_State Rocker_Right,KEY_Tigger_State Rocker_Left);
//三级灰度校准控制与显示
void Menu_Level3_Gray_Calib(KEY_Tigger_State KEY1,KEY_Tigger_State KEY2);
//三级舵机控制与显示
void Menu_Level3_Servo_Control(KEY_Tigger_State KEY1,KEY_Tigger_State KEY2,KEY_Tigger_State Rocker_UP,KEY_Tigger_State Rocker_Down);

void Menu_Level3_Task_LED(KEY_Tigger_State KEY2);			//三级运行指示灯任务显示
void Menu_Level3_Task_KEY_Sacn(KEY_Tigger_State KEY2);		//三级按键扫描任务显示
void Menu_Level3_Task_Show(KEY_Tigger_State KEY2);			//三级设置与显示任务显示
void Menu_Level3_Task_UART_Debug(KEY_Tigger_State KEY2);	//三级串口调试任务显示
void Menu_Level3_Task_Slow_Com(KEY_Tigger_State KEY2);		//三级低速计算任务显示
void Menu_Level3_Task_High_Com(KEY_Tigger_State KEY2);		//三级高速计算任务显示



void OLED_HardFault_Error_Message_Show(void);		//OLED硬件故障报错显示

#endif

