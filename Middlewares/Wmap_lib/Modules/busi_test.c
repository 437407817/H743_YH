#include "busi_test.h"
#include "cmsis_os.h"
#include "business.h"
#include "common.h"

#include "shell.h"

#define LOG_TAG "BUSI_TEST"

#include "elog.h"

TaskHandle_t mst_handle;

void mock_sample_task_entry(void *p)
{
	static uint16_t i =0;
 log_d("mock sample task is running...");
	
 while(1)
 {
	log_d("wait for sample_sem_req forever"); 
	xSemaphoreTake(busi_sample_mmc_ctrl.sem_req,portMAX_DELAY);
  
  busi_sample_values.temprature = 25;
	busi_sample_values.bright = 2;
	 
	busi_sample_values.reserved[0] = i;
  busi_sample_values.reserved[1] = i++;
	busi_sample_values.reserved[2] = i++;
  busi_sample_values.reserved[3] = i++;
	 
	log_d("current temp = %d bright = %d",busi_sample_values.temprature,busi_sample_values.bright);
	log_d("current reserved = [0]%d [1]%d [2]%d [3]%d",busi_sample_values.reserved[0],busi_sample_values.reserved[1],busi_sample_values.reserved[2],busi_sample_values.reserved[3]);

	xSemaphoreGive(busi_sample_mmc_ctrl.sem_ack);
	log_d("release sem ack");
 }
}

void mock_switch_task_entry(void *p)
{
	static uint16_t i =0;
 log_d("mock switch task is running ....");
	
 while(1)
 {
	log_d("wait for switch_sem_req forever");
	 
	xSemaphoreTake(busi_switch_mmc_ctrl.sem_req,portMAX_DELAY);
	 
  busi_switch_values.fan_status = FAN_CTRL_OPEN_STATUS;
	busi_switch_values.light_status = LIGHT_CTRL_OPEN_STATUS;
	 
	busi_switch_values.reserved[0]  = i++;
	busi_switch_values.reserved[1]  = i++;
  busi_switch_values.reserved[2]  = i++;
  busi_switch_values.reserved[3]  = i++;		

	log_d("current fan_status = %d light_status = %d",busi_switch_values.fan_status,busi_switch_values.light_status);
	log_d("current reserved = [0]%d [1]%d [2]%d [3]%d",busi_switch_values.reserved[0],busi_switch_values.reserved[1],busi_switch_values.reserved[2],busi_switch_values.reserved[3]);
	   	 
  xSemaphoreGive(busi_switch_mmc_ctrl.sem_ack);
	 
	log_d("release sem ack");
 }
}

void smst(int argc,char **argv)
{
 log_d("start mock_sample_task_entry sample task");
	
 xTaskCreate(mock_sample_task_entry,"mock_sample_task",128,(void *)0,7,&mst_handle);
 xTaskCreate(mock_switch_task_entry,"mock_switch_task",128,(void *)0,7,&mst_handle);
 
}
//ZNS_CMD_EXPORT(smst,start mock sample task)
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), smst, smst, test mock task);



