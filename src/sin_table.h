#ifndef SIN_TABLE_H
#define SIN_TABLE_H

#include "cmd_parser.h"
#include "task.h"

CMD_STATUS sin_table_cmd(int, const char *[]);
void sin_table_task_callback(void);
task_s * sin_table_get_task(void);

#endif
