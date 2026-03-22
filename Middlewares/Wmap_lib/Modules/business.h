#ifndef _BUSINESS_H_
#define _BUSINESS_H_

#include "cmsis_os.h"

#include "mmc.h"

typedef struct 
        {
				 int temprature;
				 int bright;
				 int reserved[4];
				} busi_sample_stru;
				
typedef struct 
        {
				 int fan_status;
				 int light_status;
				 int reserved[4];
				} busi_switch_stru;
				
extern mmc_ctrl_stru busi_sample_mmc_ctrl;
extern mmc_ctrl_stru busi_switch_mmc_ctrl;
				
extern busi_sample_stru busi_sample_values;
extern busi_switch_stru busi_switch_values;
				
extern void busi_process_task_entry(void *p);
				
#define BUSI_TEMPRATURE_DOWN_THRESHOLD (20)
#define BUSI_TEMPRATURE_UP_THRESHOLD (30)
#define BUSI_BRIGHT_DOWN_THRESHOLD (2)

#endif