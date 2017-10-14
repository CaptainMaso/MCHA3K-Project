#ifndef SYSID_H
#define SYSID_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include "cmd_parser.h"

CMD_STATUS sysid_motor_free_cmd(int argc, const char *argv[]);
CMD_STATUS sysid_motor_load_cmd(int argc, const char *argv[]);


#endif
