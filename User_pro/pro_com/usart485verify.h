#ifndef _USART485VERIFY_H_
#define _USART485VERIFY_H_

#include <stdint.h>

//void s485_convert_queue(uint8_t **outalldata,uint8_t** OutData,uint8_t* Datanum,uint8_t* Command_num);
void s485_convert_queue(uint8_t **outalldata,uint16_t* Command_address,uint8_t** OutData,uint8_t* Datanum);

/**
***********************************************************
* @brief USB转串口应用初始化函数
* @param
* @return 
***********************************************************
*/
void Usart485ComAppInit(void);

/**
***********************************************************
* @brief USB转串口任务处理函数
* @param
* @return 
***********************************************************
*/
void Usart485ComTask(void);

#endif
