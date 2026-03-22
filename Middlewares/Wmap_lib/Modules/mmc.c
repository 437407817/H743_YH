#include "mmc.h"
#include "string.h"

#define LOG_TAG "MMC_MODULE"
#include "elog.h"

void mmc_ctrl_init(mmc_ctrl_stru *p_mmc_ctrl,void *p_sm,int sm_len,char *sm_name)
{
 p_mmc_ctrl->sem_req = xSemaphoreCreateBinary();
 p_mmc_ctrl->sem_ack = xSemaphoreCreateBinary();
	
 p_mmc_ctrl->p_shared_mem = p_sm;
 p_mmc_ctrl->shared_mem_nbyte = sm_len;
	
 memset(p_mmc_ctrl->p_shared_mem,0,p_mmc_ctrl->shared_mem_nbyte);
	
 log_d("sem req&ack is created,shared mem called \"%s\" is initialized",sm_name);
}

int mmc_ctrl_require(mmc_ctrl_stru *p_mmc_ctrl,int waittime)
{
 xSemaphoreGive(p_mmc_ctrl->sem_req);
 log_d("sem req is gived");
 
 log_d("start waitting for sem ack"); 
 if(xSemaphoreTake(p_mmc_ctrl->sem_ack,waittime) == pdTRUE)
 {
	log_d("require SUCCESS!"); 
	log_d("shared memory is ready");
	 
	elog_hexdump("shared content",16,p_mmc_ctrl->p_shared_mem,p_mmc_ctrl->shared_mem_nbyte);
	
	return pdPASS;
 }
 else return pdFAIL;
}