///////////////////////////////////////
//	***	*** 
//	 *  **
//	***	*
//  write by Ideal_Fox
//  Affiliated to Suzhou City University
//  Revision made on July 15th, 2026
///////////////////////////////////////
//	文件介绍：
//		该文件为设置与显示源文件
///////////////////////////////////////
#include "Set_And_Show.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "User_Task.h"
#include "KEY.h"
#include "LED.h"
#include "OLED.h"
#include "Grayscale_ADC.h"
#include "Flash.h"
#include "Servo.h"

//系统菜单结构体
struct Menu Menu_Parm;

/** @brief	系统菜单控制与显示
  * @note	通过该函数，用户可使用板载按键与OLED屏幕实现查看系统部分参数以及对系统执行部分控制
  **/
void System_Menu_Control_And_Show(void)
{
	switch(Menu_Parm.Menu_Level)
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////一级菜单界面/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
		case 0:	//菜单控制
				//摇杆向下按，操作为下翻一级菜单伪指针指向选择
				if(Get_Key_State(Rocker_Down))
				{	//选择下翻
					Menu_Parm.Menu_Level_Pointer++;
					//超过长度，回到顶端
					if(Menu_Parm.Menu_Level_Pointer == 4) Menu_Parm.Menu_Level_Pointer = 0;	
				}
				//摇杆向上按，操作为上翻一级菜单伪指针指向选择
				if(Get_Key_State(Rocker_UP))
				{	//选择上翻
					Menu_Parm.Menu_Level_Pointer--;
					//超过长度，回到底端
					if(Menu_Parm.Menu_Level_Pointer < 0) Menu_Parm.Menu_Level_Pointer = 3;	
				}
				//摇杆向右短按，操作为进入当前所选择的二级菜单
				if(Get_Key_State(Rocker_Right) == short_Tigger)
				{	//菜单等级+1
					Menu_Parm.Menu_Level++;
					//二级菜单的界面为当前菜单伪指针所指向的界面
					Menu_Parm.Menu_Level_2_Interface = Menu_Parm.Menu_Level_Pointer;
					//复位菜单伪指针
					Menu_Parm.Menu_Level_Pointer = 0;
				}
		
				//菜单显示
				OLED_ShowString(0,0,"系统设置",OLED_8X16);
				OLED_ShowString(0,16,"巡线设置",OLED_8X16);
				OLED_ShowString(0,32,"舵机设置",OLED_8X16);
				OLED_ShowString(0,48,"任务查看",OLED_8X16);
				
				//伪指针指向显示,指向的参数高亮显示
				switch(Menu_Parm.Menu_Level_Pointer)
				{	
					case 0:	OLED_ReverseArea(0,0,64,16);
							break;
					case 1:	OLED_ReverseArea(0,16,64,16);
							break;
					case 2:	OLED_ReverseArea(0,32,64,16);
							break;
					case 3:	OLED_ReverseArea(0,48,64,16);
							break;
				}
				//若该界面已跳转
				if(Menu_Parm.Menu_Level != 0)
					//清除显示缓存区
					OLED_Clear();
			break;
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////二级菜单界面/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
		case 1:	switch(Menu_Parm.Menu_Level_2_Interface)
				{
//////////////////////二级系统设置菜单/////////////////////////////////////////////////////////////////////
					case 0:	//菜单控制
							//摇杆向下按，操作为下翻一级菜单伪指针指向选择
							if(Get_Key_State(Rocker_Down))
							{	//选择下翻
								Menu_Parm.Menu_Level_Pointer++;
								//超过长度，回到顶端
								if(Menu_Parm.Menu_Level_Pointer == 2) Menu_Parm.Menu_Level_Pointer = 0;	
							}
							//摇杆向上按，操作为上翻一级菜单伪指针指向选择
							if(Get_Key_State(Rocker_UP))
							{	//选择上翻
								Menu_Parm.Menu_Level_Pointer--;
								//超过长度，回到底端
								if(Menu_Parm.Menu_Level_Pointer < 0) Menu_Parm.Menu_Level_Pointer = 1;	
							}
							//摇杆向右短按，操作为进入当前所选择的三级菜单
							if(Get_Key_State(Rocker_Right) == short_Tigger)
							{	//菜单等级+1
								Menu_Parm.Menu_Level++;
								//三级菜单的界面为当前菜单伪指针所指向的界面，无偏移
								Menu_Parm.Menu_Level_3_Interface = Menu_Parm.Menu_Level_Pointer;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}
							
							//菜单显示
							OLED_ShowString(0,0,"速度设置",OLED_8X16);
							OLED_ShowString(0,16,"PID设置",OLED_8X16);
							
							//伪指针指向显示,指向的参数高亮显示
							switch(Menu_Parm.Menu_Level_Pointer)
							{	
								case 0:	OLED_ReverseArea(0,0,64,16);
										break;
								case 1:	OLED_ReverseArea(0,16,64,16);
										break;
							}
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 1)
								//清除显示缓存区
								OLED_Clear();	
						break;
//////////////////////二级巡线设置菜单/////////////////////////////////////////////////////////////////////
					case 1:	//菜单控制
						{
							static signed char Menu_Y_Shift=0;	//显示Y轴偏移
							//摇杆向下按，操作为下翻一级菜单伪指针指向选择
							if(Get_Key_State(Rocker_Down))
							{	//指针无偏移
								if(Menu_Parm.Menu_Level_Pointer < 10)
								{
									//选择下翻
									Menu_Parm.Menu_Level_Pointer++;
									//超过长度，回到顶端
									if(Menu_Parm.Menu_Level_Pointer == 5) Menu_Parm.Menu_Level_Pointer = 0;	
									//显示Y轴偏移
									if(Menu_Parm.Menu_Level_Pointer >= 3) Menu_Y_Shift = 16;
									if(Menu_Parm.Menu_Level_Pointer == 0) Menu_Y_Shift = 0;
								}
								//指针偏移为12，增加触发阈值
								else if(Menu_Parm.Menu_Level_Pointer == 13)
								{
									Grayscale.Grayscale_ADC_Trigger_Threshold += 1;
									if(Grayscale.Grayscale_ADC_Trigger_Threshold >= 100)
										Grayscale.Grayscale_ADC_Trigger_Threshold =100;
								}
							}
							//摇杆向上按，操作为上翻一级菜单伪指针指向选择
							if(Get_Key_State(Rocker_UP))
							{	//指针无偏移
								if(Menu_Parm.Menu_Level_Pointer < 10)
								{	//选择上翻
									Menu_Parm.Menu_Level_Pointer--;
									//超过长度，回到底端
									if(Menu_Parm.Menu_Level_Pointer < 0) Menu_Parm.Menu_Level_Pointer = 4;	
									//显示Y轴偏移
									if(Menu_Parm.Menu_Level_Pointer < 2) Menu_Y_Shift = 0;
									if(Menu_Parm.Menu_Level_Pointer == 4) Menu_Y_Shift = 16;
								}
								//指针偏移为12，减小触发阈值
								else if(Menu_Parm.Menu_Level_Pointer == 13)
								{
									Grayscale.Grayscale_ADC_Trigger_Threshold -= 1;
									if(Grayscale.Grayscale_ADC_Trigger_Threshold <= 0)
										Grayscale.Grayscale_ADC_Trigger_Threshold =0;
								}
							}
							//摇杆向右短按
							if(Get_Key_State(Rocker_Right) == short_Tigger && Menu_Parm.Menu_Level_Pointer < 10)
							{	//菜单伪指针在第一二栏，进入当前所选择的三级级菜单
								if(Menu_Parm.Menu_Level_Pointer == 0 || Menu_Parm.Menu_Level_Pointer == 1)
								{	//菜单等级+1
									Menu_Parm.Menu_Level++;
									//三级菜单的界面为当前菜单伪指针所指向的界面，偏移10
									Menu_Parm.Menu_Level_3_Interface = Menu_Parm.Menu_Level_Pointer+10;
									//复位菜单伪指针
									Menu_Parm.Menu_Level_Pointer = 0;
								}
								//菜单伪指针在第三栏，操作为翻转灰度计算模式
								else if(Menu_Parm.Menu_Level_Pointer == 2)
									//计算模式设置
									Grayscale.Compute_Map_Mode =! Grayscale.Compute_Map_Mode;
								//菜单伪指针在第四栏，操作为进入调整触发阈值
								else if(Menu_Parm.Menu_Level_Pointer == 3)
									//指针偏移10
									Menu_Parm.Menu_Level_Pointer += 10;
								//菜单伪指针在第五栏，操作为存储当前数据
								else if(Menu_Parm.Menu_Level_Pointer == 4)
								{	//清除显示缓存区
									OLED_Clear();
									//显示信息
									OLED_ShowString(0,16,"存储参数中。。。",OLED_8X16);
									//刷新屏幕
									OLED_Update();	
									//清除显示缓存区
									OLED_Clear();
									//延时500ms
									osDelay(500);
									
									//写入数据至flash中
									if(Flash_Write() == HAL_OK)
										OLED_ShowString(0,16,"Flash写入成功",OLED_8X16);
									else
										OLED_ShowString(0,16,"Flash写入失败",OLED_8X16);
									//刷新屏幕
									OLED_Update();	
									//清除显示缓存区
									OLED_Clear();
									//延时1500ms
									osDelay(1500);
									//退出本次循环
									return;
								}
							}
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单指针存在偏移
								if(Menu_Parm.Menu_Level_Pointer >= 10)
								//消除偏移
									Menu_Parm.Menu_Level_Pointer -=10;
								else
								{	//菜单等级-1
									Menu_Parm.Menu_Level--;
									//复位菜单指针
									Menu_Parm.Menu_Level_Pointer = 0;
								}	
							}
					
							//菜单显示
							OLED_ShowString(0,0-Menu_Y_Shift,"灰度参数查看",OLED_8X16);
							OLED_ShowString(0,16-Menu_Y_Shift,"校准设置",OLED_8X16);
							OLED_ShowString(0,32-Menu_Y_Shift,"计算模式:",OLED_8X16);
							OLED_ShowString(0,48-Menu_Y_Shift,"触发阈值:",OLED_8X16);
							OLED_ShowString(0,64-Menu_Y_Shift,"是否存储参数→",OLED_8X16);

							OLED_Printf(90,48-Menu_Y_Shift,OLED_8X16,"%3.0f",Grayscale.Grayscale_ADC_Trigger_Threshold);
							
							if(Grayscale.Compute_Map_Mode == 0)
								OLED_ShowString(80,32-Menu_Y_Shift,"正向",OLED_8X16);
							else
								OLED_ShowString(80,32-Menu_Y_Shift,"反向",OLED_8X16);
							
							//伪指针指向显示,指向的参数高亮显示
							switch(Menu_Parm.Menu_Level_Pointer)
							{	
								case 0:	OLED_ReverseArea(0,0-Menu_Y_Shift,96,16);
										break;
								case 1:	OLED_ReverseArea(0,16-Menu_Y_Shift,64,16);
										break;
								case 2:	OLED_ReverseArea(0,32-Menu_Y_Shift,64,16);
										break;
								case 3:	OLED_ReverseArea(0,48-Menu_Y_Shift,64,16);
										break;
								case 4:	OLED_ReverseArea(0,64-Menu_Y_Shift,112,16);
										break;
								case 13:OLED_ReverseArea(90,48-Menu_Y_Shift,24,16);
										break;		
							}
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 1)
							{
								//清除显示缓存区
								OLED_Clear();
								//复位Y轴偏移
								Menu_Y_Shift = 0; 
							}
								
						}
							break;
//////////////////////二级级舵机设置菜单////////////////////////////////////////////////////////////////////
					case 2:	//菜单控制
//							//摇杆向下按，操作为下翻一级菜单伪指针指向选择
//							if(Get_Key_State(Rocker_Down))
//							{	//选择下翻
//								Menu_Parm.Menu_Level_Pointer++;
//								//超过长度，回到顶端
//								if(Menu_Parm.Menu_Level_Pointer == 2) Menu_Parm.Menu_Level_Pointer = 0;	
//							}
//							//摇杆向上按，操作为上翻一级菜单伪指针指向选择
//							if(Get_Key_State(Rocker_UP))
//							{	//选择上翻
//								Menu_Parm.Menu_Level_Pointer--;
//								//超过长度，回到底端
//								if(Menu_Parm.Menu_Level_Pointer < 0) Menu_Parm.Menu_Level_Pointer = 1;	
//							}
							//摇杆向右短按，操作为进入当前所选择的三级级菜单
							if(Get_Key_State(Rocker_Right) == short_Tigger)
							{	
								//菜单等级+1
								Menu_Parm.Menu_Level++;
								//三级菜单的界面为当前菜单伪指针所指向的界面，偏移20
								Menu_Parm.Menu_Level_3_Interface = Menu_Parm.Menu_Level_Pointer+20;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}
							
							//菜单显示
							OLED_ShowString(0,0,"舵机直接控制",OLED_8X16);
							//OLED_ShowString(0,16,"舵机初始设置",OLED_8X16);
							
							
							//伪指针指向显示,指向的参数高亮显示
							switch(Menu_Parm.Menu_Level_Pointer)
							{	
								case 0:	OLED_ReverseArea(0,0,96,16);
										break;
								case 1:	OLED_ReverseArea(0,16,96,16);
										break;
							}
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 1)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////二级任务查看窗口/////////////////////////////////////////////////////////////////////
					case 3:	{
							static signed char Menu_Y_Shift=0;	//显示Y轴偏移
							//摇杆向下按
							if(Get_Key_State(Rocker_Down))
							{	//选择下翻
								Menu_Parm.Menu_Level_Pointer++;
								//超过长度，回到顶端
								if(Menu_Parm.Menu_Level_Pointer == 6) Menu_Parm.Menu_Level_Pointer = 0;	
								//菜单偏移
								if(Menu_Parm.Menu_Level_Pointer >= 3)
									Menu_Y_Shift += 16;
								//最大偏移64
								if(Menu_Y_Shift >= 32) Menu_Y_Shift = 32;
								//指针为0，偏移为0									
								if(Menu_Parm.Menu_Level_Pointer == 0) Menu_Y_Shift = 0;	
							}
							//摇杆向上按
							if(Get_Key_State(Rocker_UP))
							{	//选择上翻
								Menu_Parm.Menu_Level_Pointer--;
								//超过长度，回到底端
								if(Menu_Parm.Menu_Level_Pointer < 0) Menu_Parm.Menu_Level_Pointer = 5;
								//菜单偏移
								if(Menu_Parm.Menu_Level_Pointer <= 2)
									Menu_Y_Shift -= 16;
								//最小偏移0
								if(Menu_Y_Shift <= 0) Menu_Y_Shift = 0;
								//指针为5，偏移为32						
								if(Menu_Parm.Menu_Level_Pointer == 5) Menu_Y_Shift = 32;
							}
							//摇杆向右短按，操作为进入当前所选择的三级级菜单
							if(Get_Key_State(Rocker_Right) == short_Tigger)
							{	
								//菜单等级+1
								Menu_Parm.Menu_Level++;
								//三级菜单的界面为当前菜单伪指针所指向的界面，偏移30
								Menu_Parm.Menu_Level_3_Interface = Menu_Parm.Menu_Level_Pointer+30;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
								//复位Y轴偏移
								Menu_Y_Shift = 0;
							}

							//菜单显示
							OLED_ShowString(0,0-Menu_Y_Shift,"运行指示灯任务",OLED_8X16);
							OLED_ShowString(0,16-Menu_Y_Shift,"按键扫描任务",OLED_8X16);
							OLED_ShowString(0,32-Menu_Y_Shift,"设置与显示任务",OLED_8X16);
							OLED_ShowString(0,48-Menu_Y_Shift,"串口调试任务",OLED_8X16);
							OLED_ShowString(0,64-Menu_Y_Shift,"低速计算任务",OLED_8X16);
							OLED_ShowString(0,80-Menu_Y_Shift,"高速计算任务",OLED_8X16);

							//伪指针指向显示,指向的参数高亮显示
							switch(Menu_Parm.Menu_Level_Pointer)
							{	
								case 0:	OLED_ReverseArea(0,0-Menu_Y_Shift,112,16);
										break;
								case 1:	OLED_ReverseArea(0,16-Menu_Y_Shift,96,16);
										break;
								case 2:	OLED_ReverseArea(0,32-Menu_Y_Shift,112,16);
										break;
								case 3:	OLED_ReverseArea(0,48-Menu_Y_Shift,96,16);
										break;
								case 4:	OLED_ReverseArea(0,64-Menu_Y_Shift,96,16);
										break;
								case 5:OLED_ReverseArea(0,80-Menu_Y_Shift,96,16);
										break;		
							}
							
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 1)
								//清除显示缓存区
								OLED_Clear();
					}
							break;
				}
				break;
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////三级菜单界面/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
		case 2:	switch(Menu_Parm.Menu_Level_3_Interface)
				{
//////////////////////三级速度设置菜单///////////////////////////////////////////////////////////////////
					case 0:	
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"基础速度",OLED_8X16);
							OLED_ShowString(0,16,"转向速度",OLED_8X16);

							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
						break;
//////////////////////三级PID设置菜单///////////////////////////////////////////////////////////////////
					case 1:	
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"转速环",OLED_8X16);
							OLED_ShowString(0,16,"巡线环",OLED_8X16);

							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
						break;
//////////////////////三级灰度参数查看菜单///////////////////////////////////////////////////////////////////
					case 10://菜单控制
							//摇杆向右短按，操作为下翻一级菜单伪指针指向选择
							if(Get_Key_State(Rocker_Right) == short_Tigger)
							{	//选择下翻
								Menu_Parm.Menu_Level_Pointer++;
								//超过长度，回到顶端
								if(Menu_Parm.Menu_Level_Pointer == 2) Menu_Parm.Menu_Level_Pointer = 0;	
							}
							//摇杆向左短按，操作为上翻一级菜单伪指针指向选择
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//选择上翻
								Menu_Parm.Menu_Level_Pointer--;
								//超过长度，回到底端
								if(Menu_Parm.Menu_Level_Pointer < 0) Menu_Parm.Menu_Level_Pointer = 2 - 1;	
							}
							//摇杆中心短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Middle) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}
					
							//菜单显示
							OLED_ShowString(0,0,"校准值    实际值",OLED_8X16);
							
							if(Menu_Parm.Menu_Level_Pointer == 0)
							{	//显示实际值
								OLED_Printf(0,16,OLED_6X8,"1:%3.0f 7:%3.0f 13:%3.0f",Grayscale.Grayscale_ADC_Compute_Percent[0],Grayscale.Grayscale_ADC_Compute_Percent[6],Grayscale.Grayscale_ADC_Compute_Percent[12]);
								OLED_Printf(0,24,OLED_6X8,"2:%3.0f 8:%3.0f 14:%3.0f",Grayscale.Grayscale_ADC_Compute_Percent[1],Grayscale.Grayscale_ADC_Compute_Percent[7],Grayscale.Grayscale_ADC_Compute_Percent[13]);
								OLED_Printf(0,32,OLED_6X8,"3:%3.0f 9:%3.0f 15:%3.0f",Grayscale.Grayscale_ADC_Compute_Percent[2],Grayscale.Grayscale_ADC_Compute_Percent[8],Grayscale.Grayscale_ADC_Compute_Percent[14]);
								OLED_Printf(0,40,OLED_6X8,"4:%3.0f 10:%3.0f 16:%3.0f",Grayscale.Grayscale_ADC_Compute_Percent[3],Grayscale.Grayscale_ADC_Compute_Percent[9],Grayscale.Grayscale_ADC_Compute_Percent[15]);
								OLED_Printf(0,48,OLED_6X8,"5:%3.0f 11:%3.0f",Grayscale.Grayscale_ADC_Compute_Percent[4],Grayscale.Grayscale_ADC_Compute_Percent[10]);
								OLED_Printf(0,56,OLED_6X8,"6:%3.0f 12:%3.0f",Grayscale.Grayscale_ADC_Compute_Percent[5],Grayscale.Grayscale_ADC_Compute_Percent[11]);
							}
							else
							{	//显示实际值
								OLED_Printf(0,16,OLED_6X8,"1:%4d 7:%4d 13:%4d",Grayscale.Grayscale_ADC_Actual[0],Grayscale.Grayscale_ADC_Actual[6],Grayscale.Grayscale_ADC_Actual[12]);
								OLED_Printf(0,24,OLED_6X8,"2:%4d 8:%4d 14:%4d",Grayscale.Grayscale_ADC_Actual[1],Grayscale.Grayscale_ADC_Actual[7],Grayscale.Grayscale_ADC_Actual[13]);
								OLED_Printf(0,32,OLED_6X8,"3:%4d 9:%4d 15:%4d",Grayscale.Grayscale_ADC_Actual[2],Grayscale.Grayscale_ADC_Actual[8],Grayscale.Grayscale_ADC_Actual[14]);
								OLED_Printf(0,40,OLED_6X8,"4:%4d 10:%4d16:%4d",Grayscale.Grayscale_ADC_Actual[3],Grayscale.Grayscale_ADC_Actual[9],Grayscale.Grayscale_ADC_Actual[15]);
								OLED_Printf(0,48,OLED_6X8,"5:%4d 11:%4d",Grayscale.Grayscale_ADC_Actual[4],Grayscale.Grayscale_ADC_Actual[10]);
								OLED_Printf(0,56,OLED_6X8,"6:%4d 12:%4d",Grayscale.Grayscale_ADC_Actual[5],Grayscale.Grayscale_ADC_Actual[11]);
							}
							
							//伪指针指向显示,指向的参数高亮显示
							switch(Menu_Parm.Menu_Level_Pointer)
							{	
								case 0:	OLED_ReverseArea(0,0,48,16);
										break;
								case 1:	OLED_ReverseArea(80,0,64,16);
										break;
							}
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级校准设置菜单/////////////////////////////////////////////////////////////////////
					case 11://菜单控制
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}
							//摇杆中心长按，操作为开始校准
							if(Get_Key_State(Rocker_Middle) == long_Tigger)
							{	//最大极限值校准
								if(Menu_Parm.Menu_Level_Pointer == 0)
								{	
									//清除显示缓存区
									OLED_Clear();
									OLED_ShowString(24,8,"正在校准",OLED_8X16);
									OLED_ShowString(16,24,"最大极限值",OLED_8X16);
									OLED_Update();		//刷新屏幕
									for(;;)
									{	//校准最大极限值，校准完退出循环
										if(Grayscale_ADC_Calibration(Calibration_Max) == HAL_OK)
											break;
										//延时1ms
										osDelay(1);
									}
									//延时1000ms，留影
									osDelay(1000);
									//清除显示缓存区
									OLED_Clear();
									OLED_ShowString(24,40,"校准完成",OLED_8X16);
									OLED_ShowString(16,24,"最大极限值",OLED_8X16);
									OLED_Update();		//刷新屏幕
									//延时1500ms，留影
										osDelay(1500);
									//菜单伪指针指向选择+1
									Menu_Parm.Menu_Level_Pointer++;
									//退出
									goto out2;
								}
								//最小极限值校准
								if(Menu_Parm.Menu_Level_Pointer == 1)
								{	
									//清除显示缓存区
									OLED_Clear();
									OLED_ShowString(24,8,"正在校准",OLED_8X16);
									OLED_ShowString(16,24,"最小极限值",OLED_8X16);
									OLED_Update();		//刷新屏幕
									for(;;)
									{	//校准最大极限值，校准完退出循环
										if(Grayscale_ADC_Calibration(Calibration_Min) == HAL_OK)
											break;
										//延时1ms
										osDelay(1);
									}
									//延时1000ms，留影
									osDelay(1000);
									//清除显示缓存区
									OLED_Clear();
									OLED_ShowString(24,40,"校准完成",OLED_8X16);
									OLED_ShowString(16,24,"最小极限值",OLED_8X16);
									OLED_Update();		//刷新屏幕
									//延时1500ms，留影
										osDelay(1500);
									//菜单伪指针指向选择-1
									Menu_Parm.Menu_Level_Pointer--;
								}
								out2:
								//读取键码，以复位
								Get_Key_State(Rocker_Middle);
							}
							
							//菜单显示
							if(Menu_Parm.Menu_Level_Pointer == 0)
								OLED_ShowString(0,0,"当前校准  最大值",OLED_8X16);
							else if(Menu_Parm.Menu_Level_Pointer == 1)
								OLED_ShowString(0,0,"当前校准  最小值",OLED_8X16);
							
							//显示实际值
							OLED_Printf(0,16,OLED_6X8,"1:%4d 7:%4d 13:%4d",Grayscale.Grayscale_ADC_Actual[0],Grayscale.Grayscale_ADC_Actual[6],Grayscale.Grayscale_ADC_Actual[12]);
							OLED_Printf(0,24,OLED_6X8,"2:%4d 8:%4d 14:%4d",Grayscale.Grayscale_ADC_Actual[1],Grayscale.Grayscale_ADC_Actual[7],Grayscale.Grayscale_ADC_Actual[13]);
							OLED_Printf(0,32,OLED_6X8,"3:%4d 9:%4d 15:%4d",Grayscale.Grayscale_ADC_Actual[2],Grayscale.Grayscale_ADC_Actual[8],Grayscale.Grayscale_ADC_Actual[14]);
							OLED_Printf(0,40,OLED_6X8,"4:%4d 10:%4d16:%4d",Grayscale.Grayscale_ADC_Actual[3],Grayscale.Grayscale_ADC_Actual[9],Grayscale.Grayscale_ADC_Actual[15]);
							OLED_Printf(0,48,OLED_6X8,"5:%4d 11:%4d",Grayscale.Grayscale_ADC_Actual[4],Grayscale.Grayscale_ADC_Actual[10]);
							OLED_Printf(0,56,OLED_6X8,"6:%4d 12:%4d",Grayscale.Grayscale_ADC_Actual[5],Grayscale.Grayscale_ADC_Actual[11]);
							
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级舵机直接控制菜单///////////////////////////////////////////////////////////////////
					case 20://菜单控制
						{
							static signed char Servo_Select=0;	//舵机选择
							static signed char Menu_Y_Shift=0;	//显示Y轴偏移
							//清除显示缓存区
								OLED_Clear();
							//摇杆向下按
							if(Get_Key_State(Rocker_Down))
							{	//指针在10以内，操作为下翻一级菜单伪指针指向选择
								if(Menu_Parm.Menu_Level_Pointer < 10)
								{	//选择下翻
									Menu_Parm.Menu_Level_Pointer++;
									//超过长度，回到顶端
									if(Menu_Parm.Menu_Level_Pointer == 7) Menu_Parm.Menu_Level_Pointer = 0;	
									//菜单偏移
									if(Menu_Parm.Menu_Level_Pointer >= 3)
										Menu_Y_Shift += 16;
									//最大偏移64
									if(Menu_Y_Shift >= 48) Menu_Y_Shift = 48;
									//指针为0，偏移为0
									if(Menu_Parm.Menu_Level_Pointer == 0) Menu_Y_Shift = 0;
								}
								//指针为10，操作为切换控制的舵机
								else if(Menu_Parm.Menu_Level_Pointer == 10)
								{
									Servo_Select++;
									Servo_Select%=8;
								}
								//指针为11，操作为切换所控制的舵机的状态
								else if(Menu_Parm.Menu_Level_Pointer == 11)
									Servo_State.Servo_EN_State[Servo_Select] = !Servo_State.Servo_EN_State[Servo_Select];
								//指针为12，操作为切换所控制的舵机的状态
								else if(Menu_Parm.Menu_Level_Pointer == 12)
									Servo_State.Servo_Direction_State[Servo_Select] = !Servo_State.Servo_Direction_State[Servo_Select];
								//指针为13，当前控制的舵机角度增加
								else if(Menu_Parm.Menu_Level_Pointer == 13)
								{
									Servo_State.Servo_Angle[Servo_Select]+=1;
									if(Servo_State.Servo_Angle[Servo_Select] >= Servo_State.Servo_Angle_Max_Limit[Servo_Select])
										Servo_State.Servo_Angle[Servo_Select] = Servo_State.Servo_Angle_Max_Limit[Servo_Select];
								}
								//指针为14，当前控制的舵机最大极限角度增加
								else if(Menu_Parm.Menu_Level_Pointer == 14)
								{
									Servo_State.Servo_Angle_Max_Limit[Servo_Select]+=1;
									if(Servo_State.Servo_Angle_Max_Limit[Servo_Select] >= 180)
										Servo_State.Servo_Angle_Max_Limit[Servo_Select] = 180;
								}
								//指针为15，当前控制的舵机最小极限角度增加
								else if(Menu_Parm.Menu_Level_Pointer == 15)
								{
									Servo_State.Servo_Angle_Min_Limit[Servo_Select]+=1;
									if(Servo_State.Servo_Angle_Min_Limit[Servo_Select] >= 180)
										Servo_State.Servo_Angle_Min_Limit[Servo_Select] = 180;
								}
									
							}
							//摇杆向上按
							if(Get_Key_State(Rocker_UP))
							{	//指针在10以内，操作为上翻一级菜单伪指针指向选择
								if(Menu_Parm.Menu_Level_Pointer < 10)
								{	//选择上翻
									Menu_Parm.Menu_Level_Pointer--;
									//超过长度，回到底端
									if(Menu_Parm.Menu_Level_Pointer < 0) Menu_Parm.Menu_Level_Pointer = 6;
									//菜单偏移
									if(Menu_Parm.Menu_Level_Pointer <= 2)
										Menu_Y_Shift -= 16;
									//最小偏移0
									if(Menu_Y_Shift <= 0) Menu_Y_Shift = 0;
									//指针为6，偏移为48
									if(Menu_Parm.Menu_Level_Pointer == 6) Menu_Y_Shift = 48;
								}
								//指针为10，操作为切换控制的舵机
								else if(Menu_Parm.Menu_Level_Pointer == 10)
								{
									Servo_Select--;
									if(Servo_Select < 0) Servo_Select = 7;
								}
								//指针为11，操作为切换所控制的舵机的状态
								else if(Menu_Parm.Menu_Level_Pointer == 11)
									Servo_State.Servo_EN_State[Servo_Select] = !Servo_State.Servo_EN_State[Servo_Select];
								//指针为12，操作为切换所控制的舵机的状态
								else if(Menu_Parm.Menu_Level_Pointer == 12)
									Servo_State.Servo_Direction_State[Servo_Select] = !Servo_State.Servo_Direction_State[Servo_Select];
								//指针为13，当前控制的舵机角度减小
								else if(Menu_Parm.Menu_Level_Pointer == 13)
								{
									Servo_State.Servo_Angle[Servo_Select]-=1;
									if(Servo_State.Servo_Angle[Servo_Select] <= Servo_State.Servo_Angle_Min_Limit[Servo_Select])
										Servo_State.Servo_Angle[Servo_Select] = Servo_State.Servo_Angle_Min_Limit[Servo_Select];
								}
								//指针为14，当前控制的舵机最大极限角度减小
								else if(Menu_Parm.Menu_Level_Pointer == 14)
								{
									Servo_State.Servo_Angle_Max_Limit[Servo_Select]-=1;
									if(Servo_State.Servo_Angle_Max_Limit[Servo_Select] <= 0)
										Servo_State.Servo_Angle_Max_Limit[Servo_Select] = 0;
								}
								//指针为15，当前控制的舵机最小极限角度减小
								else if(Menu_Parm.Menu_Level_Pointer == 15)
								{
									Servo_State.Servo_Angle_Min_Limit[Servo_Select]-=1;
									if(Servo_State.Servo_Angle_Min_Limit[Servo_Select] <= 0)
										Servo_State.Servo_Angle_Min_Limit[Servo_Select] = 0;
								}
							}
							//摇杆向左短按
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//指针在10以内，操作为退出当前菜单
								if(Menu_Parm.Menu_Level_Pointer < 10)
								{	//菜单等级-1
									Menu_Parm.Menu_Level--;
									//复位菜单伪指针
									Menu_Parm.Menu_Level_Pointer = 0;
									//复位舵机选择与偏移
									Servo_Select = 0;
									Menu_Y_Shift = 0;
								}
								//指针不在10以内，操作指针内偏移10
								else 
									Menu_Parm.Menu_Level_Pointer -= 10;
							}
							
							//摇杆向右短按，且指针在10以内，操作为进入数据调整
							if(Get_Key_State(Rocker_Right) == short_Tigger && Menu_Parm.Menu_Level_Pointer < 10)
							{	
								//指针外偏移10
								Menu_Parm.Menu_Level_Pointer += 10;
								//若偏移为16，则为指向数据存储命令
								if(Menu_Parm.Menu_Level_Pointer == 16)
								{	//清除显示缓存区
									OLED_Clear();
									//显示信息
									OLED_ShowString(0,16,"存储参数中。。。",OLED_8X16);
									//刷新屏幕
									OLED_Update();	
									//清除显示缓存区
									OLED_Clear();
									//延时500ms
									osDelay(500);
									
									//写入数据至flash中
									if(Flash_Write() == HAL_OK)
										OLED_ShowString(0,16,"Flash写入成功",OLED_8X16);
									else
										OLED_ShowString(0,16,"Flash写入失败",OLED_8X16);
									//刷新屏幕
									OLED_Update();	
									//清除显示缓存区
									OLED_Clear();
									//延时1500ms
									osDelay(1500);
									//操作指针内偏移10
									Menu_Parm.Menu_Level_Pointer -= 10;
									//退出本次循环
									return;
								}
							}
					
							//菜单显示
							OLED_ShowString(0,0 - Menu_Y_Shift,"舵机编号  舵机",OLED_8X16);
							OLED_ShowString(0,16 - Menu_Y_Shift,"舵机状态",OLED_8X16);
							OLED_ShowString(0,32 - Menu_Y_Shift,"舵机方向",OLED_8X16);
							OLED_ShowString(0,48 - Menu_Y_Shift,"舵机角度",OLED_8X16);
							OLED_ShowString(0,64 - Menu_Y_Shift,"最大角度",OLED_8X16);
							OLED_ShowString(0,80 - Menu_Y_Shift,"最小角度",OLED_8X16);
							OLED_ShowString(0,96 - Menu_Y_Shift,"是否存储参数→",OLED_8X16);
							
							//参数显示
							//舵机编号
							OLED_ShowNum(112,0 - Menu_Y_Shift,Servo_Select+1,1,OLED_8X16);
							//舵机状态
							if(Servo_State.Servo_EN_State[Servo_Select] == Servo_Disable)
								OLED_ShowString(80,16 - Menu_Y_Shift,"失能",OLED_8X16);
							else
								OLED_ShowString(80,16 - Menu_Y_Shift,"使能",OLED_8X16);
							//舵机方向
							if(Servo_State.Servo_Direction_State[Servo_Select] == Forward)
								OLED_ShowString(80,32 - Menu_Y_Shift,"正向",OLED_8X16);
							else
								OLED_ShowString(80,32 - Menu_Y_Shift,"反向",OLED_8X16);
							//舵机角度
							OLED_Printf(80,48 - Menu_Y_Shift,OLED_8X16,"%3.0f",Servo_State.Servo_Angle[Servo_Select]);
							//最大角度
							OLED_Printf(80,64 - Menu_Y_Shift,OLED_8X16,"%3.0f",Servo_State.Servo_Angle_Max_Limit[Servo_Select]);
							//最小角度
							OLED_Printf(80,80 - Menu_Y_Shift,OLED_8X16,"%3.0f",Servo_State.Servo_Angle_Min_Limit[Servo_Select]);
							
							//伪指针指向显示,指向的参数高亮显示
							switch(Menu_Parm.Menu_Level_Pointer)
							{	
								case 0:	OLED_ReverseArea(0,0 - Menu_Y_Shift,64,16);
										break;
								case 1:	OLED_ReverseArea(0,16 - Menu_Y_Shift,64,16);
										break;
								case 2:	OLED_ReverseArea(0,32 - Menu_Y_Shift,64,16);
										break;
								case 3:	OLED_ReverseArea(0,48 - Menu_Y_Shift,64,16);
										break;
								case 4:	OLED_ReverseArea(0,64 - Menu_Y_Shift,64,16);
										break;
								case 5:	OLED_ReverseArea(0,80 - Menu_Y_Shift,64,16);
										break;
								case 6:	OLED_ReverseArea(0,96 - Menu_Y_Shift,112,16);
										break;
								case 10:OLED_ReverseArea(80,0 - Menu_Y_Shift,40,16);
										break;
								case 11:OLED_ReverseArea(80,16 - Menu_Y_Shift,32,16);
										break;
								case 12:OLED_ReverseArea(80,32 - Menu_Y_Shift,32,16);
										break;
								case 13:OLED_ReverseArea(80,48 - Menu_Y_Shift,24,16);
										break;
								case 14:OLED_ReverseArea(80,64 - Menu_Y_Shift,24,16);
										break;
								case 15:OLED_ReverseArea(80,80 - Menu_Y_Shift,24,16);
										break;

							}
					
						}
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级运行指示灯任务查看//////////////////////////////////////////////////////////////////
					case 30://菜单控制
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"任务名称",OLED_8X16);
							OLED_ShowString(0,16,"RUN_LED_Flash",OLED_8X16);
							OLED_ShowString(0,32,"最小剩余栈",OLED_8X16);
							OLED_Printf(0,48,OLED_8X16,"%d",xTaskDetails_RUN_LED_Flash.usStackHighWaterMark);

							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级按键扫描任务查看///////////////////////////////////////////////////////////////////
					case 31://菜单控制
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"任务名称",OLED_8X16);
							OLED_ShowString(0,16,"KEY_Scan",OLED_8X16);
							OLED_ShowString(0,32,"最小剩余栈",OLED_8X16);
							OLED_Printf(0,48,OLED_8X16,"%d",xTaskDetails_KEY_Scan.usStackHighWaterMark);
							
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级设置与显示任务查看//////////////////////////////////////////////////////////////////
					case 32://菜单控制
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"任务名称",OLED_8X16);
							OLED_ShowString(0,16,"Set_And_Show",OLED_8X16);
							OLED_ShowString(0,32,"最小剩余栈",OLED_8X16);
							OLED_Printf(0,48,OLED_8X16,"%d",xTaskDetails_Set_And_Show.usStackHighWaterMark);
							
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级串口调试任务查看///////////////////////////////////////////////////////////////////
					case 33://菜单控制
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"任务名称",OLED_8X16);
							OLED_ShowString(0,16,"UART_Debug",OLED_8X16);
							OLED_ShowString(0,32,"最小剩余栈",OLED_8X16);
							OLED_Printf(0,48,OLED_8X16,"%d",xTaskDetails_UART_Debug.usStackHighWaterMark);
							
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级低速计算任务查看///////////////////////////////////////////////////////////////////
					case 34://菜单控制
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"任务名称",OLED_8X16);
							OLED_ShowString(0,16,"Slow_Compute",OLED_8X16);
							OLED_ShowString(0,32,"最小剩余栈",OLED_8X16);
							OLED_Printf(0,48,OLED_8X16,"%d",xTaskDetails_Slow_Compute.usStackHighWaterMark);
							
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;
//////////////////////三级高速计算任务查看///////////////////////////////////////////////////////////////////
					case 35://菜单控制
							//摇杆向左短按，操作为退出当前菜单
							if(Get_Key_State(Rocker_Left) == short_Tigger)
							{	//菜单等级-1
								Menu_Parm.Menu_Level--;
								//复位菜单伪指针
								Menu_Parm.Menu_Level_Pointer = 0;
							}

							//菜单显示
							OLED_ShowString(0,0,"任务名称",OLED_8X16);
							OLED_ShowString(0,16,"High_Compute",OLED_8X16);
							OLED_ShowString(0,32,"最小剩余栈",OLED_8X16);
							OLED_Printf(0,48,OLED_8X16,"%d",xTaskDetails_High_Compute.usStackHighWaterMark);
							
							//若该界面已跳转
							if(Menu_Parm.Menu_Level != 2)
								//清除显示缓存区
								OLED_Clear();
							break;

						
				}
			break;
		//四级菜单界面
		case 3:
			break;
	}
	
	
	OLED_Update();		//刷新屏幕
	//清除显示缓存区
	OLED_Clear();
}



/** @brief	OLED硬件故障报错显示
  * @note	若出现硬件故障，在屏幕上进行显示，并同步开启PD3指示灯
  * @note	该函数在"stm32f4xx_it.c"文件中，由HardFault_Handler中断函数调用
  **/
void OLED_HardFault_Error_Message_Show(void)
{
	PD3_LED_Control(LED_ON);							//开启PD3
	OLED_ShowString(1,1,"HardFault_Error",OLED_8X16);	//输出报错故障码
	OLED_Update();										//刷新屏幕
}
