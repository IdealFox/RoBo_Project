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
		+-二级任务查看窗口(3)---+-三级运行指示灯任务查看(30)
								+-三级按键扫描任务查看(31)
								+-三级设置与显示任务查看(32)
								+-三级串口调试任务查看(33)
								+-三级低速计算任务查看(34)
								+-三级高速计算任务查看(35)
*/

//系统菜单结构体
struct Menu{
	signed char Menu_Level;				//菜单当前等级
	
	signed char Menu_Level_1_Interface;	//一级菜单界面码，默认为0，即只存在一个一级菜单
	signed char Menu_Level_2_Interface;	//二级菜单界面码
	signed short Menu_Level_3_Interface;//三级菜单界面码
	signed short Menu_Level_4_Interface;//四级菜单界面码
	signed short Menu_Level_5_Interface;//五级菜单界面码
	
	signed char Menu_Level_Pointer;		//菜单伪指针
};
extern struct Menu Menu_Parm;



void System_Menu_Control_And_Show(void);		//系统菜单控制与显示



void OLED_HardFault_Error_Message_Show(void);		//OLED硬件故障报错显示

#endif

