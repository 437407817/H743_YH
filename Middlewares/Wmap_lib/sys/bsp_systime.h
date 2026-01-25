#ifndef __SYSCFG_TIME_H
#define	__SYSCFG_TIME_H


#include "stm32_FH_xxx_hal.h"

#include "FreeRTOS.h"
#include "task.h"







#if  (USE_FREERTOS ==0)

#define   Delay_Us(ms)   Delay_Us_NoTask(ms)	
#endif

#define  Delay(ms)  delay_ms(ms)


#define   Delay_Ms(ms)   delay_ms(ms)

																				









/* 获取内核时钟频率 */
#define GET_CPU_ClkFreq()       HAL_RCC_GetSysClockFreq()
#define SysClockFreq            (480000000)
/* 为方便使用，在延时函数内部调用CPU_TS_TmrInit函数初始化时间戳寄存器，
   这样每次调用函数都会初始化一遍。
   把本宏值设置为0，然后在main函数刚运行时调用CPU_TS_TmrInit可避免每次都初始化 */  

#define CPU_TS_INIT_IN_DELAY_FUNCTION   0  


/*******************************************************************************
 * 函数声明
 ******************************************************************************/
uint32_t CPU_TS_TmrRd(void);
//HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
uint32_t HAL_GetTick(void);

//使用以下函数前必须先调用CPU_TS_TmrInit函数使能计数器，或使能宏CPU_TS_INIT_IN_DELAY_FUNCTION
//最大延时值为8秒
void CPU_TS_Tmr_Delay_US(uint32_t us);
#define HAL_Delay(ms)     CPU_TS_Tmr_Delay_US(ms*1000)
#define CPU_TS_Tmr_Delay_S(s)       CPU_TS_Tmr_Delay_MS(s*1000)






extern __IO uint32_t NowUse_rtos_delay;

//#define  delay_ms(ms)                   do{\
//                                          if(USE_FREERTOS==0){\
//																						Delay_Ms_NoTask(ms);\
//                                          }else{vTaskDelay(ms/portTICK_PERIOD_MS); }\
//                                          }while(0)
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

void Delay_Ms_NoTask( __IO uint32_t ms);
void Delay_Us_NoTask( __IO uint32_t us);



//Delay_Ms_NoTask(ms);}
void SYSTEM_set_rtos_running(void);

void delay_us_soft(uint32_t delay_us);

void TimingDelay_Decrement(void);

void DelayNclock(__IO uint32_t nCount);









#endif /* __SYSCFG_TIME_H */
