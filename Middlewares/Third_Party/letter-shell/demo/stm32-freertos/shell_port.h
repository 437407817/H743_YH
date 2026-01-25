/**
 * @file shell_port.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__

//#include "serial.h"
#include "shell.h"
#include "./usart/bsp_usart_shell.h"
extern Shell shell;
int userShellMutexUnlock(void);
void LetterShell_OS_Init(void);
void letterShell_no_os_WhileInit(void);
void letter_Shell_NoOsWhileTask(void);
void letter_Shell_NoOsHandleTask(void);



void HAL_UART_Shell_ErrorCallback(UART_HandleTypeDef *huart);
void HAL_UART_Shell_RxCpltCallback(UART_HandleTypeDef *huart);
#endif
