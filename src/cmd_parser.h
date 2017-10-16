#ifndef CMD_PARSER_H
#define CMD_PARSER_H

typedef enum{
	CMD_OK,
	CMD_UNKNOWN,
	CMD_INVALIDPARAM,
	CMD_REPORTEDERR
} CMD_STATUS;

typedef struct {
	const char * cmd_string;
	CMD_STATUS (*cmd_function)(int argc, const char* argv[]);
	//const char * help_string;
} command_s;

void cmd_parse(int argc, const char* argv[]);

#endif
