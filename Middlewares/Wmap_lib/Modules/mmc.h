#ifndef _MMC_H_
#define _MMC_H_

#include "cmsis_os.h"

typedef struct 
        {
				 SemaphoreHandle_t sem_req;
				 SemaphoreHandle_t sem_ack;
					
				 void *p_shared_mem;
				 int shared_mem_nbyte;
				} mmc_ctrl_stru;
				
#define MMC_DELAY_TIME (10)
				
extern void mmc_ctrl_init(mmc_ctrl_stru *p_mmc_ctrl,void *p_sm,int sm_len,char *sm_name);
extern int mmc_ctrl_require(mmc_ctrl_stru *p_mmc_ctrl,int waittime);	

#endif