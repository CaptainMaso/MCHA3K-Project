#ifndef DATA_LOG_H
#define DATA_LOG_H

#include "cmd_parser.h"

#define MAX_DEVICES 10

typedef enum {
    LOG_CUR_ML	= 0,
	LOG_CUR_MR	= 1,
    LOG_ENC_ML	= 2,
	LOG_ENC_MR	= 3,
	LOG_IMU_AX	= 4,
	LOG_IMU_AY	= 5,
	LOG_IMU_AZ	= 6,
	LOG_IMU_GX	= 7,
	LOG_IMU_GY	= 8,
	LOG_IMU_GZ	= 9,
} LOG_DEVICE_T;

CMD_STATUS log_cmd(int, const char *[]);
void log_task_function(void);

#endif
