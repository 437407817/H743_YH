/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   GPIO输出--使用固件库点亮LED灯
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32_FH_xxx.h"
#include "./sys/sys_init_all.h" 
#include "./user_config.h"
#include "FreeRTOS.h"
#include "freerotspro.h"  
#include "./sys/sysio.h"
#include "./DataCache/data_stack.h"


#define NVIC_VTOR_MASK       0x3FFFFF80
#define APP_START_ADDR        0x08004000
#define APP_MAX_SIZE         0x00014000   //80KB
void SystemClock_Config(void);

uint8_t sys_stm32_clock_init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq);


void ota_app_vtor_reconfig(void)
{
    /* Set the Vector Table base location by user application firmware definition */
    SCB->VTOR = APP_START_ADDR & NVIC_VTOR_MASK;
}

int main(void)
{   
	
	
	

	
	
	
		HAL_Init();  

  SystemClock_Config();
//	sys_stm32_clock_init(160, 5, 2, 4); 
stack_init_marker();// 初始化栈监控,必须放main中，否则跳出方法后卡死
//	stack_fill(); // 初始化时填充栈（必须在其他函数调用前执行）
	//ota_app_vtor_reconfig();
//__enable_irq();
	// 在系统初始化（SystemClock_Config）后添加

	
  ALL_Init_cfg();
	    // 打印各总线频率
    SYSTEM_INFO("SYSCLK: %lu MHz\r\n", HAL_RCC_GetSysClockFreq()/1000000);
    SYSTEM_INFO("HCLK:   %lu MHz\r\n", HAL_RCC_GetHCLKFreq()/1000000);
    SYSTEM_INFO("APB1:   %lu MHz\r\n", HAL_RCC_GetPCLK1Freq()/1000000);
    SYSTEM_INFO("APB2:   %lu MHz\r\n", HAL_RCC_GetPCLK2Freq()/1000000);
		
		
	NeedToOnceOperate_Pro();

	
	
#if USE_FREERTOS 
	FreerTosProBegin();
#endif
	
	
// 局部变量，用于保存转换计算后的电压值 	 

	
  while (1)
  {
		
	
NeedToRepeatOperate_Pro();

		//Delay(1000);  

  }
}

//void Delay(__IO uint32_t nCount)
//{
//  for(; nCount != 0; nCount--);
//}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** 启用电源配置更新
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** 配置主内稳压器输出电压
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** 初始化CPU、AHB和APB总线时钟
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		while(1);
  }
  /** 初始化CPU、AHB和APB总线时钟
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
		while(1);
  }
}






uint8_t sys_stm32_clock_init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_ClkInitTypeDef rcc_clk_init_handle;
    RCC_OscInitTypeDef rcc_osc_init_handle;

    MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);                         /*使能供电配置更新 */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  /* VOS = 1, Scale1, 1.2V内核电压,FLASH访问可以得到最高性能 */
    while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY);     /* 等待电压稳定 */

    /* 使能HSE，并选择HSE作为PLL时钟源，配置PLL1，开启USB时钟 */
    rcc_osc_init_handle.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
    rcc_osc_init_handle.HSEState = RCC_HSE_ON;
    rcc_osc_init_handle.HSIState = RCC_HSI_OFF;
    rcc_osc_init_handle.CSIState = RCC_CSI_OFF;
    rcc_osc_init_handle.HSI48State = RCC_HSI48_ON;
    rcc_osc_init_handle.PLL.PLLState = RCC_PLL_ON;
    rcc_osc_init_handle.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    rcc_osc_init_handle.PLL.PLLN = plln;
    rcc_osc_init_handle.PLL.PLLM = pllm;
    rcc_osc_init_handle.PLL.PLLP = pllp;
    rcc_osc_init_handle.PLL.PLLQ = pllq;
    rcc_osc_init_handle.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    rcc_osc_init_handle.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    rcc_osc_init_handle.PLL.PLLFRACN = 0;
    ret = HAL_RCC_OscConfig(&rcc_osc_init_handle);
    if (ret != HAL_OK)
    {
        return 1;
    }

    /*
     *  选择PLL的输出作为系统时钟
     *  配置RCC_CLOCKTYPE_SYSCLK系统时钟,400M
     *  配置RCC_CLOCKTYPE_HCLK 时钟,200Mhz,对应AHB1，AHB2，AHB3和AHB4总线
     *  配置RCC_CLOCKTYPE_PCLK1时钟,100Mhz,对应APB1总线
     *  配置RCC_CLOCKTYPE_PCLK2时钟,100Mhz,对应APB2总线
     *  配置RCC_CLOCKTYPE_D1PCLK1时钟,100Mhz,对应APB3总线
     *  配置RCC_CLOCKTYPE_D3PCLK1时钟,100Mhz,对应APB4总线
     */
    rcc_clk_init_handle.ClockType = (RCC_CLOCKTYPE_SYSCLK \
                                    | RCC_CLOCKTYPE_HCLK \
                                    | RCC_CLOCKTYPE_PCLK1 \
                                    | RCC_CLOCKTYPE_PCLK2 \
                                    | RCC_CLOCKTYPE_D1PCLK1 \
                                    | RCC_CLOCKTYPE_D3PCLK1);

    rcc_clk_init_handle.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    rcc_clk_init_handle.SYSCLKDivider = RCC_SYSCLK_DIV1;
    rcc_clk_init_handle.AHBCLKDivider = RCC_HCLK_DIV2;
    rcc_clk_init_handle.APB1CLKDivider = RCC_APB1_DIV2; 
    rcc_clk_init_handle.APB2CLKDivider = RCC_APB2_DIV2; 
    rcc_clk_init_handle.APB3CLKDivider = RCC_APB3_DIV2;  
    rcc_clk_init_handle.APB4CLKDivider = RCC_APB4_DIV2; 
    ret = HAL_RCC_ClockConfig(&rcc_clk_init_handle, FLASH_LATENCY_2);
    if (ret != HAL_OK)
    {
        return 1;
    }

    HAL_PWREx_EnableUSBVoltageDetector();   /* 使能USB电压电平检测器 */
    __HAL_RCC_CSI_ENABLE() ;                /* 使能CSI时钟 */
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;         /* 使能SYSCFG时钟 */
    HAL_EnableCompensationCell();           /* 使能IO补偿单元 */
    return 0;
}






//void vApplicationTickHook( void ){
//	lv_tick_inc(1);
//}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
