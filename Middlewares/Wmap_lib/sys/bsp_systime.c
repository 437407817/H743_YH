/**
  ******************************************************************************
  * @file    bsp_util_time.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  



#include "./sys/bsp_systime.h"   
#include "./core_cm7.h" 
#include "./Sys/sys_dwt.h"
//******************************
__IO uint32_t NowUse_rtos = 0;

void SYSTEM_set_rtos_running(void)
{
    NowUse_rtos = 1;
}

void delay_ms(uint32_t ms){
if(NowUse_rtos==0){

Delay_Ms_NoTask(ms);
}else{

vTaskDelay(ms/portTICK_PERIOD_MS);
}

}




void delay_us(uint32_t us){

DWT_Delay_us(us);
}






//----------------

static __IO uint32_t TimingDelay;

void delay_us_soft(uint32_t delay_us)
{
  volatile unsigned int num;
  volatile unsigned int t;


  for (num = 0; num < delay_us; num++)
  {
    t = 11;
    while (t != 0)
    {
      t--;
    }
  }
}



/**
  * @brief   us延时程序,10us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
  * @retval  无
  */
void Delay_Ms_NoTask(__IO uint32_t ms)
{ 
    uint32_t i;
    for(i = 0; i < ms; i++)
    {
        while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)==0);
    }

//		  TimingDelay = ms;
//  while(TimingDelay != 0);
}









/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
***/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}


void Delay_Us_NoTask( __IO uint32_t us)
{
delay_us_soft(us);
}


void DelayNclock(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}












#define  DWT_CR      *(__IO uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
#define  DEM_CR      *(__IO uint32_t *)0xE000EDFC


#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


/**
  * @brief  初始化时间戳
  * @param  无
  * @retval 无
  * @note   使用延时函数前，必须调用本函数
  */
//HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
//{
//    /* 使能DWT外设 */
//    DEM_CR |= (uint32_t)DEM_CR_TRCENA;                

//    /* DWT CYCCNT寄存器计数清0 */
//    DWT_CYCCNT = (uint32_t)0u;

//    /* 使能Cortex-M DWT CYCCNT寄存器 */
//    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
//  
//    return HAL_OK;
//}

/**
  * @brief  读取当前时间戳
  * @param  无
  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
  */
uint32_t CPU_TS_TmrRd(void)
{        
  return ((uint32_t)DWT_CYCCNT);
}

/**
  * @brief  读取当前时间戳
  * @param  无
  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
  */
uint32_t HAL_GetTick(void)
{        
  return ((uint32_t)DWT_CYCCNT/SysClockFreq*1000);
}



/**
  * @brief  采用CPU的内部计数实现精确延时，32位计数器
  * @param  us : 延迟长度，单位1 us
  * @retval 无
  * @note   使用本函数前必须先调用CPU_TS_TmrInit函数使能计数器，
            或使能宏CPU_TS_INIT_IN_DELAY_FUNCTION
            最大延时值为8秒，即8*1000*1000
  */
void CPU_TS_Tmr_Delay_US(uint32_t us)
{
  uint32_t ticks;
  uint32_t told,tnow,tcnt=0;

  /* 在函数内部初始化时间戳寄存器， */  
#if (CPU_TS_INIT_IN_DELAY_FUNCTION)  
  /* 初始化时间戳并清零 */
  HAL_InitTick(5);
#endif
  
  ticks = us * (GET_CPU_ClkFreq() / 1000000);  /* 需要的节拍数 */      
  tcnt = 0;
  told = (uint32_t)CPU_TS_TmrRd();         /* 刚进入时的计数器值 */

  while(1)
  {
    tnow = (uint32_t)CPU_TS_TmrRd();  
    if(tnow != told)
    { 
        /* 32位计数器是递增计数器 */    
      if(tnow > told)
      {
        tcnt += tnow - told;  
      }
      /* 重新装载 */
      else 
      {
        tcnt += UINT32_MAX - told + tnow; 
      } 
      
      told = tnow;

      /*时间超过/等于要延迟的时间,则退出 */
      if(tcnt >= ticks)break;
    }  
  }
}










//void Delay_Us_NoTask( __IO uint32_t us)
//{
//	uint32_t i;
//	//SysTick_Config(SystemCoreClock/1000000);
//	
//	for(i=0;i<us;i++)
//	{
//		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
//		while( !((SysTick->CTRL)&(1<<16)) );
//	}
//	// 关闭SysTick定时器
//	//SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
//}

////void SysTick_Delay_Ms( __IO uint32_t ms)
//void Delay_Ms_NoTask( __IO uint32_t ms)
//{
//	uint32_t i;	
//	//SysTick_Config(SystemCoreClock/1000);
//	
//	for(i=0;i<ms;i++)
//	{
//		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
//		// 当置1时，读取该位会清0
//		while( !((SysTick->CTRL)&(1<<16)) );
//	}
//	// 关闭SysTick定时器
//	//SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
//}














#if 0
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
#endif



// sys_time.c 文件（新建）
#include <time.h>  // 必须包含

// 全局时间变量（供外部修改，比如RTC/定时器中断）
time_t g_sys_time = 0;

// 标准time()函数实现（无static，全局可见）
time_t time(time_t *t)
{
    // 核心逻辑：返回当前系统时间
    if (t != NULL) {
        *t = g_sys_time;
    }
    return g_sys_time;
}

// 可选：提供时间更新接口（供定时器/RTC调用）
void sys_time_inc(void)
{
    g_sys_time++;  // 每秒+1，模拟时间流逝
}





/*********************************************END OF FILE**********************/
