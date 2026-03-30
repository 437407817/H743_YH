#ifndef __GV_VARIABLE_H
#define	__GV_VARIABLE_H


//#include "stm32fxxx.h"
//#include "string.h"
//#include <time.h>
#include "./stm32_FH_xxx_hal.h"


//#define BOARD_COUNT  8  //不许变
//#define BOARD_COUNT_CHANGE  4  //可变
//#define BATTERY_COUNT  12

//#define EEPROM_SettingSaveDataCount  sizeof(GV_ScreenSettingDataState)
//#define EEPROM_SYS_SettingSaveDataCount 6


//#define ASK_SLAVE_NOTWORK_DUTY_TIME   2000
//#define ASK_SLAVE_WORK_DUTY_TIME   4000


//typedef struct ScreenCommu_ITEM
//{
//uint16_t    ScreenAddr;
//uint16_t*   ScreenData;//必须指向一个地址，不能*p=数，因为指针是空指针。
//uint16_t    ScreenDataSize;

//}ScreenCommu_Item_t;





//typedef struct AskUploadDataS
//{
//uint8_t Addr;// FE=所有从控板停止工作开始采样，同时主控变接收状态。
//	uint8_t DataType;//数据类型 0=数据；1=参数；2=校正； 
//}str_AskUploadDataS;


//typedef struct JZ_S
//{
//uint8_t Addr;
//uint8_t Reserve;
//int16_t PWMOffset[12];
//uint16_t SetCCurrent[12];//mA
//uint16_t SetDCurrent[12]; //mA
//uint16_t TrueSampleCCurrent [12]; //mA
//uint16_t TrueSampleDCurrent [12]; //mA
//uint16_t TrueVoltage[12];// //mV
//}str_JZ_S;










//typedef struct 
//{
////uint8_t Board_id;
////uint8_t bat_id;
//uint16_t bat_vol[12];
//#if 0	
//uint16_t bat_current[BATTERY_COUNT];
//uint16_t bat_vol_Uncalibate[BATTERY_COUNT];		
//uint16_t bat_current_Uncalibate[BATTERY_COUNT];
//uint16_t bat_Vol_SET_Calibration_factor[BATTERY_COUNT];
//uint16_t bat_Current_SET_Calibration_factor[BATTERY_COUNT];	
//#endif
//	
//uint16_t  SumParallel_vol_Value;/*< 采集的的总电压 (暂时不计入计算)*/	
//	#if 1	
//uint16_t  Max_vol_Value;   /*< 采集的最高电压 */
//uint16_t	Min_vol_Value;	  /*< 采集的最低电压 */
//uint16_t  MaxDiff_vol_Value; /*< 压差电压 */
//uint16_t  Average_vol_Value; /*< 平均电压 */	
//	#endif

//#if 0	
//uint8_t  board_ReadyRunState;//启动状态/*< 1可以启动状态2通信超时0不可启动 */
//uint8_t  bat_RunCount;/*< 该板运行电池数量 */
//uint32_t ParrleBatterycapacityAs[BATTERY_COUNT];		//容量可正（充电）负（放电）
//uint32_t ParrleBatteryChargeSecond[BATTERY_COUNT];		//充电时间	
//uint16_t ParrleBatterycapacityAh[BATTERY_COUNT];		//容量可正（充电）负（放电）

//uint8_t	 ParrleBatteryCapacityPeriod;  //容量计算周期	
//uint8_t  Nop2;	
//#endif
//}str_BoardBatteryState;



///**
//  * @brief  屏幕设置参数，（独立）
//  * @param  
//  * @retval 屏幕发过来的数据，直接改变这里，然后再改变板内数据（分开目的是解耦）
//  */
//typedef struct {
////1********************************//
//uint16_t   Set_ChargeStateSelect[8];//0停止，1充电，2并放电，3维护////
//uint16_t   Set_ChargeStateSelect_resume[8];
//uint16_t   Set_NowSingleRunState[8];	//0 1
////uint16_t   Set_NowSingleRunState_OneButtonRun;//1启动，0停止，
//uint16_t   Set_NowRunState;	//1启动，0停止，//Set_NowSingleRunState_OneButtonRun
//	
//	//*************************
////uint16_t   Set_Bat_OldBattNum[8];
////1***********************************//
//uint8_t   Set_NowShowPageRegionNum;		//当前屏幕显示的哪页区域（不是哪页，是区域）
//uint8_t  Set_NowShowPageNum;
//	
//uint8_t Set_NowShowLanguage;//中英文

//uint16_t  Set_BoardCount;/*< 充电板数量 */
//	
//uint16_t  Set_BoardBat01;/*< 充电板数量 */	
//uint16_t  Set_BoardBat02;/*< 充电板数量 */		
//uint16_t  Set_BoardBat03;/*< 充电板数量 */	
//uint16_t  Set_BoardBat04;/*< 充电板数量 */
//uint16_t  Set_BoardBat05;/*< 充电板数量 */	
//uint16_t  Set_BoardBat06;/*< 充电板数量 */		
//uint16_t  Set_BoardBat07;/*< 充电板数量 */	
//uint16_t  Set_BoardBat08;/*< 充电板数量 */


//uint16_t 	Set_Bat_UpperLimitParallelVol[12];//并充截止电压 mV
//uint16_t  Set_Bat_UpperAlarmParallelVol[12];	//并充报警上限电压 mV
//uint16_t 	Set_Bat_LowerLimitParallelVol[12];//并放截止电压 mV
//uint16_t  Set_Bat_LowerAlarmParallelVol[12];	//并放报警上限电压 mV	

//uint16_t  Set_Bat_BalanceVol_xmV[12];/*< 充电电压 */
//uint16_t  Set_Bat_BalanceCurrent_xmA[12];/*< 均衡电流 */
//uint16_t  Set_Bat_ChargeCurrent_xmA[12];/*< 充电电流 */
//uint16_t  Set_Bat_DischargeCurrent_xmA[12];/*< 放电电流 */
	


//}str_ScreenSetRunState;


//typedef struct {
////1********************************//
//uint8_t   PowerDown_State;//0停止，1充电，2并放电，3维护////3串充，4串放 5恒压并充(11)
//uint8_t   PowerDown_BoardNowRunBit;

//uint8_t   PowerDown_resumeState[8];
////-------
//uint8_t	  powerdown_first_enter;	//0:第一次进入 1:第n次进入
//}str_PowerDownState;



//typedef struct {
//uint16_t    sys_NOW_year;

//uint16_t    sys_NOW_month;
//uint16_t    sys_NOW_day;
//uint16_t    sys_NOW_hour;
//uint16_t    sys_NOW_minute;	
//uint16_t    sys_NOW_second;	
//	
//	
//}str_Screen_SYS_SHOW_SettingDataState;


//typedef struct {

/////*****************
//uint16_t    sys_sd_year;

//uint16_t    sys_sd_month;
//uint16_t    sys_sd_day;
//uint16_t    sys_sd_hour;
//uint16_t    sys_sd_minute;	
//uint16_t    sys_sd_second;	

//uint16_t    sys_sd_save_hour;
//uint16_t    sys_sd_save_minute;	
//uint16_t    sys_sd_save_second;	
/////*****************

//uint8_t  sys_sd_tmp_flag;	//第一次进入系统设置界面，需要刷新<时间设置>
//}str_Screen_SYS_SettingDataState;

//typedef struct {

/////*****************
//uint8_t    sys_DS_second;		
//uint8_t    sys_DS_minute;	
//uint8_t    sys_DS_hour;	
//uint8_t    sys_DS_day;	
//uint8_t    sys_DS_month;	
//uint8_t    sys_DS_year;


//}str_DS1302_data;





//typedef struct {

/////*****************
//uint16_t    src_DS_year;

//uint16_t    src_DS_month;
//uint16_t    src_DS_day;
//uint16_t    src_DS_hour;
//uint16_t    src_DS_minute;	
//uint16_t    src_DS_second;	

//uint16_t    src_sd_save_refresh_hour;//sd刷新间隔
//uint16_t    src_sd_save_refresh_minute;	
//uint16_t    src_sd_save_refresh_second;	
/////*****************

//uint8_t  sys_DS_tmp_flag;	//第一次进入系统设置界面，需要刷新<时间设置>
//}str_Screen_DS_SettingDataState;



//typedef struct {



//uint8_t  SD_launch_flag;	//机器启动时为1，结束时为0,记录sd启动结束
//	
//	
//}str_SD_data;











//typedef struct DS1302_time_ITEM
//{

//uint16_t    DS_year;

//uint16_t    DS_month;
//uint16_t    DS_day;
//uint16_t    DS_hour;
//uint16_t    DS_minute;	
//uint16_t    DS_second;		

////uint16_t    DS_cal_year;//未使用
////uint16_t    DS_cal_month;
////uint16_t    DS_cal_day;
////uint16_t    DS_cal_hour;
////uint16_t    DS_cal_minute;	
////uint16_t    DS_cal_second;		
////	
////	
////	
////time_t      DS_timestamp;
////time_t      DS_begin_timestamp;	
////time_t      DS_TimestampDifference;
////char        DS_work_state[9];
//}DS_time_ITEM_t;


//typedef struct {

//uint16_t    EEP_DS_save_hour;
//uint16_t    EEP_DS_save_minute;	
//uint16_t    EEP_DS_save_second;	
//	
////uint16_t    EEP_sd_sum_seconds;
//}str_EEprom_SYS_SettingDataState;




//typedef struct {


//uint16_t Set_Bat_SingleBattRun[8];  // B0=1：第1支电池启动；B1=1：第2
//uint16_t Set_Bat_Run_Vol_difference[8]; //压差
//uint16_t Set_Bat_RunCount_polar[8]; //均衡次数，最多设3次
//uint16_t Set_Bat_Runworkingmode[8];//0=停止；1=恒流并充；2=并放；3=维护；
//uint16_t Set_Bat_BattNum[8];/*< 单板充电电池数量 */

//uint16_t 	Set_Bat_UpperLimitParallelVol[8];//并充截止电压 mV
//uint16_t  Set_Bat_UpperAlarmParallelVol[8];	//并充报警上限电压 mV

//uint16_t 	Set_Bat_LowVoltageAlarm[8];
//uint16_t  Set_Bat_ChargeCurrent_xmA[8];/*< 充电电流 */

//uint16_t  Set_Bat_StopCapacity[8];//	//容量截止 0.1Ah	
//uint16_t  Set_Bat_StopTime[8];//时间截止
//}str_ScreenSettingDataState;





//typedef struct {

//uint16_t EEP_Bat_SingleBattRun[8];
//uint16_t EEP_Bat_RunVol_difference[8];
//uint16_t EEP_Bat_RunCount_polar[8];
//uint16_t EEP_Bat_Run[8];//0=停止；1=并充；2=并放；3=维护；4=串充；5=串放；

//uint16_t  EEP_Bat_BattNum[8];/*< 单板充电电池数量  */

//uint16_t 	EEP_Bat_UpperLimitParallelVol[8];//并充截止电压 mV
//uint16_t  EEP_Bat_UpperAlarmParallelVol[8];	//并充报警上限电压 mV

//uint16_t 	EEP_Bat_LowVoltageAlarm[8];
//uint16_t  EEP_Bat_BalanceCurrent_xmA[8];/*< 均衡电流 */

//uint16_t  EEP_Bat_StopCapacity[8];//	//容量截止 0.1Ah		
//uint16_t  Set_Bat_StopTime[8];//时间截止

//}str_EEpromSettingState;



//typedef struct 
//{
//uint8_t CommS_Addr;//0XFE=广播地址
//uint8_t Effective;//保留//B0:1=第一单元报文有效，0=无效；B1：1=第二单元报文有效，0=无效；B2：1=第三单元报文有效，0=无效；B3：1=第四报文有效，0=无效  5
//uint16_t SingleBattRun[8];
//uint8_t Vol_difference[8];
//uint8_t RunCount_polar[8];
//uint8_t S_Run[8];//0=停止；1=并充；2=并放；3=维护；
//uint8_t BattNum[8];//共12组(每个板子电池个数)

//uint16_t StopParalleltChargeVol[8];//共12组并充截止电压 mV
//uint16_t AlarmParalleltChargeVol[8];//共12组并充报警上限电压 mV
////uint16_t StopParalleltDischargeVol[8];//共12组并放截止电压 mV
////uint16_t AlarmParalleltDischargeVol[8];//共12组并放报警上限电压 mV

//uint16_t 	com_LowVoltageAlarm[8];
////uint16_t BalanceVol[8];//共12组均衡电压 mv
////	
////uint16_t BalanceCur[8];//共12组均衡电流 mA
//	
//uint16_t ChargeCurrent[8];//并充电流 mA
////	
////uint16_t DischargeCurrent[8];//并放电流 mA

//uint16_t StopCapacity[8];//	//容量截止 0.1Ah

//uint16_t StopTime[8];//	//时间截止 0.1Ah

//}str_CommSendToSlaveBoardState;







//typedef struct 
//{
//uint8_t A_Addr;// FE=所有从控板停止工作开始采样，同时主控变接收状态。
//uint8_t A_DataType;//数据类型 0=数据；1=参数；2=校正；
//}str_CommSlaveAskState;





//#define ETE_Size   8


//typedef struct 
//{

//uint16_t SC_Vol[12];//mV
//int16_t SC_Cur[12];//正=充电；负=放电，0.01A
//int16_t SC_Cap[12];//容量Ah，正=充；负=放，0.1Ah
//uint16_t SC_BattWrokingStatus;//电池工作状态。B0：1=并充，0=未工作；……；B7：1=并充，0=未工作
//uint16_t SC_BattDischargeStatus;//电池并放状态。B0：1=并放，0=未工作；……；B7：1=并放 ，0=未工作
//uint16_t SC_BattCompleteStatus;//电池工作状态。B0：1=完成；0=未完成；……；B7：1=完成，0=未完成

//uint16_t SC_SUMvol;
//#if 1
//uint16_t SC_Maxvol;	
//uint16_t SC_Minvol;		
//uint16_t SC_MaxDiffvol;	
//	
//#endif
	
//uint16_t SC_RuningIntervalTime;	//已运行（充电次数、放电数次、均衡数次）的次数间隔倒计时的时间
//uint16_t SC_RuningCount;		//已运行（充电次数、放电数次、均衡数次）的次数

//uint16_t SC_ErrorBattNo;//故障电池号1~8
//uint16_t SC_ErrorType[4];//故障类型。0=正常；1=并充过压；2=并放欠压；3=电池断线；4=并充过流；5=并放过流；6=串充过流；7=串充过压；8=串放过流；9=串放过压
//uint16_t SC_ErrorType_English[ETE_Size];	


//uint16_t SC_BitControl;	
//	
//	
//}str_ScreenBatVolComuState;



//typedef struct 
//{
//uint8_t BS_Addr;//1~12
//uint8_t BS_WorkStatus;//0=停止；1=并充；2=并放；3=维护； 6=并充完成；7=并放完成；8=维护完成；

//uint8_t RuningIntervalTime ;//已运行（充电次数、放电数次、均衡数次）的次数间隔倒计时的时间
//uint8_t RuningCount;//已运行（充电次数、放电数次、均衡数次）的次数

//uint16_t BS_Vol[12];//mV
//int16_t BS_Cur[12];//正=充电；负=放电，0.01A
//int16_t BS_Cap[12];//容量Ah，正=充；负=放，0.1Ah
//int16_t BS_Energy[12];//能量kwh，正=充；负=放，0.1kwh
//uint16_t BS_BattWrokingStatus;//电池工作状态。B0：1=并充，0=未工作；……；B7：1=并充，0=未工作
//uint16_t BS_BattDischargeStatus;//电池并放状态。B0：1=并放，0=未工作；……；B7：1=并放 ，0=未工作
//uint16_t BS_BattCompleteStatus;//电池工作状态。B0：1=完成；0=未完成；……；B7：1=完成，0=未完成
//uint8_t BS_ErrorBattNo;//故障电池号1~8
//uint8_t BS_ErrorType;//故障类型。0=正常；1=并充过压；2=并放欠压；3=电池断线；4=并充过流；5=并放过流；6=串充过流；7=串充过压；8=串放过流；9=串放过压
//uint8_t BS_BitControl1;//B0：0=正序，1=倒序
//uint8_t BS_BitControl2;//
//}str_GetSlaveUpdloadState;


//typedef struct 
//{

////uint16_t SC_Capacity[12];//
//uint16_t SC_Energy[12];//



//}str_ScreenBatCapComuState;



//typedef struct {
//str_GetSlaveUpdloadState getSlaveUpdloadState[BOARD_COUNT];
//str_ScreenBatVolComuState screenBatVolComuState[BOARD_COUNT];  //发送给屏幕的
//str_ScreenBatCapComuState  screenBatCapComuState[BOARD_COUNT];
////uint16_t  com_   ///多少次接收从机数据未收到中，则报某机通信故障.
//uint8_t	commu_overtime[BOARD_COUNT];
////uint8_t commu_ErrorOvertimeBOARD;   ///0通信正常,其他：板号
//uint16_t commu_ErrorBatteryNo;	///故障电池号
////uint16_t commu_SC_ErrorType[4];	//其他故障类型	故障类型。0=正常；1=并充过压；2=并放欠压；3=电池断线；4=并充过流；5=并放过流；6=串充过流；7=串充过压；8=串放过流；9=串放过压

//	
//uint16_t commu_ErrorBoardNo;	///故障板号	
//uint16_t commu_MachineStateMessage[4];	///机器工作信息	
//uint16_t commu_MachineStateMessage_English[ETE_Size];	
////-----------------
//uint8_t	commu_OT_scrShowErrorMess_flag[BOARD_COUNT];	//超时时间标识，目的是在超时时间里放入超时信息，屏蔽以免与正常模式方法冲突
//uint16_t commu_F_ErrorBoardNo_FLAG[BOARD_COUNT];//故障板号是否被使用	
//uint16_t commu_F_ErrorBoardNo_EnterInFLAG[BOARD_COUNT];	
//	
//uint16_t	commu_overtime_FLAG;	

//uint16_t	commu_needfinishwork_FLAG;	//启动哪个，置1哪个
//	uint16_t	commu_finishwork_Old_FLAG;	//完成一个，置1一个
//uint16_t	commu_finishwork_FLAG;	//完成一个，置1一个
//	
//	
//uint16_t	check_ErrorType_Nomal_FLAG;		//进入4次正常状态，则无通信获取异常
//uint16_t	check_isOverTimeStop[BOARD_COUNT];
//	
//uint8_t	  check_TimeStop_BoardNo;	//未使用

////uint8_t	  check_overtime_askdata_timesflag[BOARD_COUNT];	//超时次数	
////uint16_t	check_overtime_askdata;	//超时时间（第一次发送询问数据到返回数据的时间）

//}str_ComuBoardState;




//typedef struct {

//str_BoardBatteryState  batterystate[BOARD_COUNT];
////str_SetBoardState  setboardstate[BOARD_COUNT];


//uint8_t hard_board_count;
//uint8_t hard_battery_count;

//uint8_t hard_board_whichIsActiveBit;//哪一个板被激活00001111
//uint8_t hard_board_HightActiveBitNum;//最高位在第几位
//uint8_t hard_board_nowActive;	//当前机器被激活板（1-4，4秒一次）或0（4秒中都是0）现在被激活哪个板，用了通讯询问下一组数据

//	
//uint8_t  HW_NeedWorkingState[8]; 	/*<需要运行的 0停止，1充 2 放   3维护   (排除暂停)4=串充；5=串放；6=暂停；7=继续/恢复*/
//uint8_t  HW_NowRealWorkingState[8];        /*<已经开始运行的 0停止，1充 2 放   3维护  (排除暂停) 4=串充；5=串放；6=暂停；7=继续/恢复*/

//uint8_t  HW_NeedAllWorkingState; 	
//uint8_t ReadyRunState;	/*< 1可以启动状态2通信超时0不可启动 */

//	
//	
//}str_HardwareState;






/**
  * @brief  485设置校正参数，（独立）
  * @param  
  * @retval 
  */
//typedef struct 
//{
//uint8_t cal_Addr;
//uint8_t Reserve;	
//	


//uint16_t BX_DisplaySampleChargeCurrent[12];/*< 显示充电电流 */	
//uint16_t BX_TrueSampleChargeCurrent[12];/*< 显示放电电流 */		

//uint16_t BX_DisplaySampleDchargeCurrent[12];/*< 仪表充电电流 */		
//uint16_t BX_TrueSampleDchargeCurrent[12];/*< 仪表放电电压 */	
//	
//uint16_t BX_TrueVoltage[12];/*< 仪表电压 */	

////uint16_t B0X_S12_VolCalibrationOn;/*<校准电压 */	
////uint16_t B0X_S12_VolResetOn;/*<重置校准电压 */
////uint16_t B0X_S12_ChargeCurCalibrationOn;/*<校准充电电流 */	
////uint16_t B0X_S12_ChargeCurResetOn;/*<重置校准充电电流 */
////uint16_t B0X_S12_DisChargeCurCalibrationOn;/*<校准充电电流 */	
////uint16_t B0X_S12_DisChargeCurResetOn;/*<重置校准充电电流 */	

//}str_BatterySetCalibration;


//typedef struct 
//{

//uint16_t S_Rectify_ShowChargeCur[12];/*< 显示充电电流 */	


//uint16_t S_Rectify_TrueSampleChargeCurrent[12];/*< 真实充电电流 */		
//uint16_t S_Rectify_TrueSampleChargeCurrentEnable;/*< 真实充电电流 */	
//uint16_t S_Rectify_TrueSampleChargeCurrentReset;/*< 真实充电电流 */	

//uint16_t S_Rectify_DisplaySampleDchargeCurrent[12];/*< 显示放电电流 */		


//uint16_t S_Rectify_TrueSampleDchargeCurrent[12];/*< 仪表放电电压 */	
//uint16_t S_Rectify_TrueSampleDchargeCurrentEnable;/*< 仪表放电电压 */	
//uint16_t S_Rectify_TrueSampleDchargeCurrentReset;/*< 仪表放电电压 */	

//uint16_t S_Rectify_TrueVoltage[12];/*< 仪表电压 */	
//uint16_t S_Rectify_TrueVoltageEnable;/*< 仪表电压 */		
//uint16_t S_Rectify_TrueVoltageReset;/*< 仪表电压 */	

//}str_ScreenSingleCalibration;

//typedef struct 
//{

//str_ScreenSingleCalibration S_SingleCalibration[BOARD_COUNT]; 
//uint16_t Rectify_pagenum;
//uint16_t Rectify_data_integrality;	//校正前后数据完整性标识
//	
//}str_ScreenAllCalibration;



//typedef struct {

/////*****************
//uint8_t    sys_DS_second;		
//uint8_t    sys_DS_minute;	
//uint8_t    sys_DS_hour;	
//uint8_t    sys_DS_day;	
//uint8_t    sys_DS_month;	
//uint8_t    sys_DS_year;


//}str_DS1302_data;

//typedef struct {
//	
//uint8_t ChipID1[4];
//uint8_t ChipID2[4];
//uint8_t ChipID3[3];	
//	
//uint8_t ChipisActive;	//1激活，2未激活可用，3

//uint8_t overtime_year;
//uint8_t overtime_month;	
//uint8_t overtime_day;

////char code[40];

//}str_ChipEncode_data;




//typedef struct {
//	
//str_ChipEncode_data chipdata;
//	
//char code[16];

//}str_ChipEncode;


//typedef struct {
//	
////uint8_t ChipID1[4];
////uint8_t ChipID2[4];
////uint8_t ChipID3[3];	
//	
//uint16_t Srceen_ChipisActive;	//1激活，2未激活可用，3
//uint16_t Srceen_overtime_year;
//uint16_t Srceen_overtime_month;	
//uint16_t Srceen_overtime_day;
//	

////char code[40];

//}str_SrceenChipEncode_data;



//把chipid和到期时间组合，加密，发给用户

//typedef struct SD_485_stru_ITEM
//{
////char        timestring[9];
//char        sd_work_state[BOARD_COUNT][9];
//}str_SD_485_ITEM_t;



//*************************************************************************




















//#define MaxMaintainBatteryNum 24
//#define UnconnectBatteryVOL   50  /*< 未连接电池电压 50mv */


















//typedef struct ScreenBatteryShow_Item
//{
//uint16_t S_Battery_24_VOL[24];
//uint16_t S_ParrleBatteryState[24];
//uint16_t S_ParrleChatState[2];

//uint16_t S_ParrleBatterycapacityAh[24];


//	
//uint16_t S_SumSerialVol;
//uint16_t S_SingleMaxVol;
//uint16_t S_SingleMinVol;	
//uint16_t S_MaxDiffVol;	
//uint16_t S_SerialCurrent;		
//	
//uint16_t S_R1A_S2A;	
//uint8_t S_Run_Mintues;		
//uint8_t S_Run_Hour;		

//uint16_t S_SerialorParrel;	
//uint16_t S_RunState;   /*< 0停止1启动2暂停 */	
//	
//uint8_t  S_Maintain_Num;	/*< 维护电池数量 */		
//uint8_t  S_Maintain_Diffvol;	/*< 维护压差 */
//uint8_t	 S_Delay_Mintues;	 /*< 并充延迟分钟 */
//uint8_t	 S_Delay_Seconds;	 /*< 并充延迟秒 */
//	
//	
////uint16_t S_Run_HourMintues;		
//uint16_t S_Delay_MintuesSeconds;






//}ScreenBatteryShow_Item_t;

//未启动，串充中，延迟中，并充中，暂停中

//typedef struct ScreenBatterySet_Item{
//uint16_t  Ss_SumUpperlimitValue;
//uint16_t  Ss_SingleUpperlimitValue;
//uint16_t  Ss_SingleLowerlimitValue; 
//uint16_t  Ss_MaintainNum; 
//uint16_t  Ss_UpperAlarmValue;
//uint16_t  Ss_lowerAlarmValue;	
//uint16_t  Ss_SerialorParrel;	
//uint16_t  Ss_R1A_S2A;
//uint16_t  Ss_Delay_Mintues;
//uint16_t  Ss_Delay_Seconds;	
//}ScreenBatterySet_Item_t;




//typedef struct Test_ITEM
//{
//uint16_t    t01;

//}Test_Item_t;







//#define MAXBatteryNum  24




//void initGV_variable(void);
//uint8_t checkNeedStateNotEquRealState(void);
//void Setboadnumfun(void);



#define BATNUM 64


typedef struct GET_VOL_Data_ITEM
{
	
uint64_t     Bat_WorkStatus;
uint16_t    Bat_Vol[BATNUM];	

}STR_GET_VOL_Data_t;


typedef struct SEND_RUNSTATE_ITEM
{
	
uint8_t    Machine_RUN;
uint8_t    Reserver;	

}STR_SEND_RUN_DATA_t;



typedef struct SEND_SETTING_STATE_ITEM
{
	
uint8_t   IsAutoRun;
uint8_t   Reserver;	
uint16_t 	StopVoltage;
uint16_t 	VoltageDifference;	
	

}STR_SEND_SETTING_DATA_t;







//typedef struct SD_485_stru_ITEM
//{
//	
//uint8_t     Bat_WorkStatus[8];
//uint16_t    Bat_Vol[64];	

//}STR_GET_VOL_Data_t;
































#endif /* __BASICMOTION_H */

