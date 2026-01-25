/**
  ******************************************************************************
  * @file    SysTick/TimeBase/main.h 
  * @author  MCD Application Team
  * @version V3.6.0
  * @date    20-September-2021
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

/* Includes ------------------------------------------------------------------*/
//#include "FreeRTOSConfig.h" 
//#include "freerotspro.h" 




#define USE_FREERTOS                    1 //¿ªÆôfreertos


/* Exported types ------------------------------------------------------------*/
#if  USE_FREERTOS  

#define USE_OS  1
#endif



#define USE_LVGL 1


#ifndef USE_OS
#define USE_OS  0
#define USE_LVGL_OS 0
#else

#define USE_LVGL_OS 1
#endif


#define USE_UART_DMA 0
#define USE_UART_DMA_RX 0
#define USE_UART_DMA_TX 1


#if USE_OS
#define USE_LETTER_SHELL 1
#else
#define USE_LETTER_SHELL 1
#endif



#define USE_SERIAL_IT 0




#if (USE_LETTER_SHELL)
#define USE_SHELL 1
#else
#define USE_SHELL 0
#endif




#define USE_TOUCH_IIC 0






//#define HAL_GPIO_MODULE_ENABLED
//#define HAL_I2C_MODULE_ENABLED






/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __USER_CONFIG_H */

