#ifndef DATA_LOG_H
#define DATA_LOG_H

#include "cmd_parser.h"

#define MAX_DEVICES 5

CMD_STATUS log_cmd(int, const char *[]);
void log_task_function(void);

#endif
