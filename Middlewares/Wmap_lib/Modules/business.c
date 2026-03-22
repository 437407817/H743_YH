#include "cmsis_os.h"
#include "business.h"

#include "mmc.h"
#include "common.h"

#define LOG_TAG "BUSI_MODULE"
#include "elog.h"

busi_sample_stru busi_sample_values;
busi_switch_stru busi_switch_values;

mmc_ctrl_stru busi_sample_mmc_ctrl;
mmc_ctrl_stru busi_switch_mmc_ctrl;


void busi_process_task_entry(void *p)
{
 log_d("busi_process_task is running ...");
	
 mmc_ctrl_init(&busi_sample_mmc_ctrl,&busi_sample_values,sizeof(busi_sample_values),"busi_sample_values");
 mmc_ctrl_init(&busi_switch_mmc_ctrl,&busi_switch_values,sizeof(busi_switch_values),"busi_switch_values");
	
 while(1)
 {
	if(mmc_ctrl_require(&busi_sample_mmc_ctrl,MMC_DELAY_TIME) == pdPASS)
	{
	 log_d("sample requre SUCCESS!");
		
	 log_i("temprature : %d , bright : %d , rsvd0 : %d , rsvd1 : %d , rsvd2 : %d , rsvd3 : %d ",    \
		                                                              busi_sample_values.temprature,  \
		                                                              busi_sample_values.bright,      \
		                                                              busi_sample_values.reserved[0], \
		                                                              busi_sample_values.reserved[1], \
		                                                              busi_sample_values.reserved[2], \
		                                                              busi_sample_values.reserved[3]  \
		                                                             );
		
	 if(busi_sample_values.temprature < BUSI_TEMPRATURE_DOWN_THRESHOLD)
	 {
		log_w("temprature is too LOW!!");
		busi_switch_values.fan_status = FAN_CTRL_CLOSE_STATUS;
	 }
	 
	 if(busi_sample_values.temprature > BUSI_TEMPRATURE_UP_THRESHOLD)
	 {
		log_w("temprature is too HIGH!!");
		busi_switch_values.fan_status = FAN_CTRL_OPEN_STATUS;		
	 }
	 
	 if(busi_sample_values.bright < BUSI_BRIGHT_DOWN_THRESHOLD)
	 {
		log_w("bright is too LOW!!");
		busi_switch_values.light_status = LIGHT_CTRL_OPEN_STATUS;		
	 }
	 
	 if(mmc_ctrl_require(&busi_switch_mmc_ctrl,MMC_DELAY_TIME) == pdPASS)
	 {
		log_d("switch requre SUCCESS!");
		 
	  log_i("fan_status-- : %d , light_status : %d , rsvd0 : %d , rsvd1 : %d , rsvd2 : %d , rsvd3 : %d ",    \
		                                                                     busi_switch_values.fan_status,  \
		                                                                     busi_switch_values.light_status,\
		                                                                     busi_switch_values.reserved[0], \
		                                                                     busi_switch_values.reserved[1], \
		                                                                     busi_switch_values.reserved[2], \
		                                                                     busi_switch_values.reserved[3]  \
		                                                                    );
	 } 
	 else log_e("switch requre FAIL!");		
	}
	else
	{
	 log_i("sample values requre FAIL!");
	}
	
	vTaskDelay(5000);
 }
}