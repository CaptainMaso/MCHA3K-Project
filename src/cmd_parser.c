#include <stdio.h>
#include <stdlib.h> // atoi
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>
#include "cmd_line_buffer.h"
#include "potentiometer.h"
#include "encoder.h"
#include "led.h"
#include "cmd_parser.h"
#include "controller.h"
#include "sin_table.h"
#include "log_data.h"

#define UNUSED(x) (void)(x)

static CMD_STATUS _cmd_help(int argc, const char* argv[]);
static CMD_STATUS _print_chip_pinout(int argc, const char* argv[]);
//static CMD_STATUS pot_cmd(int argc, const char* argv[]);
//static CMD_STATUS enc_cmd(int argc, const char* argv[]);
//static CMD_STATUS led_cmd(int argc, const char* argv[]);
static CMD_STATUS set_cmd(int argc, const char* argv[]);
static CMD_STATUS get_cmd(int argc, const char* argv[]);
static CMD_STATUS mulxy_cmd(int argc, const char* argv[]);
static CMD_STATUS ctrl_cmd(int argc, const char* argv[]);

static CMD_STATUS timercount_cmd(int argc, const char* argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf_P(PSTR("triggered: %"PRIu16), tasks_triggered());
	return CMD_OK;
}

static float _x = 0.0f;
static float _y = 0.0f;

float vref, theta, v = 0.0f;

static const command_s command_list[] =
{
		{"help", &_cmd_help, "help [cmd]		Prints the help string of [cmd], else all functions and help string"},
		{"pinout", &_print_chip_pinout, "pinout [pin]		Prints the pinout of the ATMEGA32P, [pin] prints the specific functions of each pin"},
		/*{"pot", &pot_cmd, "pot		Prints the current ADC reading of the potentiometer"},
		{"enc", &enc_cmd, "enc [reset]		Prints the current reading of the encoder, or resets the encoder count to 0"},
		{"led", &led_cmd, "led [on|off|<value>]		Returns the current LED value, or changes the current value"},*/
		{"set", &set_cmd, "set TODO"},
		{"get", &get_cmd, "get TODO"},
		{"mulxy", &mulxy_cmd, "mulxy TODO"},
		{"ctrl", &ctrl_cmd, "ctrl TODO"},
		{"sin", &sin_table_cmd, "sin <samples> <amplitude> <frequency>"},
		{"log", &log_cmd, "log [pot|enc] <samples>"},
		{"timercount", &timercount_cmd, "log [pot|enc] <samples>"}
};

#ifdef NO_LD_WRAP
void cmd_parse(uint8_t argc, const char* argv[]) __asm__("___real_cmd_parse");
#endif

void cmd_parse(int argc, const char* argv[])
{

	if (argc == 0)
	{
		printf_P(PSTR("ERROR: Tried to parse 0 arguments\n"));
	}
	else
	{
		CMD_STATUS err = CMD_UNKNOWN;
		for(int i = 0; i < (int)(sizeof(command_list)/sizeof(command_s)); i++)
		{
			if (!strcmp(command_list[i].cmd_string, argv[0]))
			{
				err = command_list[i].cmd_function(argc - 1, &(argv[1]));
				break;
			}
		}

		switch(err)
		{
		case CMD_INVALIDPARAM:
			printf_P(PSTR("\tERROR: Invalid Parameter\n"));
			break;
		case CMD_UNKNOWN:
			printf_P(PSTR("\tERROR: Unknown command \"%s\"\n"), argv[0]);
			break;
		case CMD_OK:
		default:
			break;
		}

	}
}

CMD_STATUS _cmd_help(int argc, const char* argv[])
{
    // Describe argument syntax using POSIX.1-2008 convention
    // see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html

	if (argc == 0)
	{
		for (uint8_t i = 0; i < sizeof(command_list)/sizeof(command_s); i++)
		{
			printf_P(PSTR("\t%s\n"), command_list[i].help_string);
		}
		return CMD_OK;
	}
	else
	{
		for(uint8_t i = 0; i < sizeof(command_list)/sizeof(command_s); i++)
		{
			if (!strcmp(command_list[i].cmd_string, argv[0]))
			{
				printf_P(PSTR("\t%s\n"), command_list[i].help_string);
				return CMD_OK;
			}
		}
		return CMD_INVALIDPARAM;
	}

}

CMD_STATUS _print_chip_pinout(int argc, const char* argv[])
{
	UNUSED(argc);
	UNUSED(argv);
    printf_P(PSTR(
        "Pin configuration:\n"
        "\n"
        "                               .----. ,----.\n"
        "Mot Right B:    (XCK/T0)PB0 -->|1    \"   40|<-- PA0(ADC0)  : MR GND\n"
        " Mot Left B:        (T1)PB1 -->|2        39|<-- PA1(ADC1)  : MR CurSns\n"
        "MoRight Dir: (INT2/AIN0)PB2 <--|3        38|<-- PA2(ADC2)  : ML GND\n"
        "MotLeft Dir:  (OC0/AIN1)PB3 <--|4    A   37|<-- PA3(ADC3)  : ML CurSns\n"
        "           :       (!SS)PB4 <->|5    T   36|<-> PA4(ADC4)  :\n"
        "                  (MOSI)PB5 <->|6    M   35|<-> PA5(ADC5)  :\n"
        "                  (MIS.O)PB6 <->|7    E   34|<-> PA6(ADC6)  :\n"
        "                   (SCK)PB7 <->|8    L   33|--> PA7(ADC7)  : Active LED Flash\n"
        "                     !RESET -->|9        32|<-- AREF\n"
        "                        VCC ---|10   A   31|--- GND\n"
        "                        GND ---|11   T   30|--- AVCC\n"
        "                      XTAL2 <--|12   m   29|<-> PC7(TOSC2) :\n"
        "                      XTAL1 -->|13   e   28|<-> PC6(TOSC1) :\n"
        " USB/BT RX :       (RXD)PD0 -->|14   g   27|<-> PC5(TDI)   :\n"
        " USB/BT TX :       (TXD)PD1 <--|15   a   26|<-> PC4(TDO)   :\n"
        "Mot Right A:      (INT0)PD2 -->|16   3   25|<-> PC3(TMS)   :\n"
        " Mot Left A:      (INT1)PD3 -->|17   2   24|<-- PC2(TCK)   :\n"
        "MotRight EN:      (OC1B)PD4 <--|18       23|<-> PC1(SDA)   : TWI (I2C) Data\n"
        "Mot Left EN:      (OC1A)PD5 <--|19       22|<-> PC0(SCL)   : TWI (I2C) Clk\n"
        "           :      (ICP1)PD6 <->|20       21|<-> PD7(OC2)   :\n"
        "                               `-----------'\n"
    ));
    return CMD_OK;
}


/*CMD_STATUS pot_cmd(int argc, const char* argv[])
{
	UNUSED(argv);
	if (argc == 0)
	{
		printf_P(PSTR("Potentiometer ADC value is %" PRIu16 "\n"), pot_get_value());
		return CMD_OK;
	}
	else
		return CMD_INVALIDPARAM;
}

static CMD_STATUS enc_cmd(int argc, const char* argv[])
{
	if (argc == 0)
	{
		printf_P(PSTR("Encoder count is %" PRId32 "\n"), encoder_get_count());
		return CMD_OK;
	}
	else if (argc == 1)
	{
		if (!strcmp(argv[0], "reset"))
		{
			encoder_set_count(0);
			printf_P(PSTR("Encoder count reset to 0\n"));
			return CMD_OK;
		}
		else
		{
			return CMD_INVALIDPARAM;
		}
	}
	else return CMD_INVALIDPARAM;
}

static CMD_STATUS led_cmd(int argc, const char* argv[])
{
	if (argc == 0)
	{
		printf_P(PSTR("LED brightness is %" PRIu8 "\n"), led_get_brightness());
		return CMD_OK;
	}
	else if (argc == 1)
	{
		if (!strcmp_P(argv[0], PSTR("on")))
		{
			led_on();
			printf_P(PSTR("LED is on\n"));
			return CMD_OK;
		}
		else if (!strcmp_P(argv[0], PSTR("off")))
		{
			led_off();
			printf_P(PSTR("LED is off\n"));
			return CMD_OK;
		}
		else
		{
			char* end;
			int32_t val = strtol(argv[0], &end, 10);
			uint8_t led  = (val > 255) ? 255 : ((val < 0) ? 0 : val);
			if (*end != '\0')
				return CMD_INVALIDPARAM;
			else
			{
				led_set_brightness(led);
				printf_P(PSTR("LED brightness set to %" PRIu8 "\n"), led);
				return CMD_OK;
			}
		}
	}
	else
		return CMD_INVALIDPARAM;
}
*/
static CMD_STATUS set_cmd(int argc, const char* argv[])
{
	if (argc == 2)
	{
		if (!strcmp_P(argv[0], PSTR("x")))
		{
			_x = atof(argv[1]);
		}
		else if (!strcmp_P(argv[0], PSTR("y")))
		{
			_y = atof(argv[1]);
		}
		else if (!strcmp_P(argv[0], PSTR("vref")))
		{
			vref = atof(argv[1]);
		}
		else if (!strcmp_P(argv[0], PSTR("v")))
		{
			v = atof(argv[1]);
		}
		else if (!strcmp_P(argv[0], PSTR("theta")))
		{
			theta = atof(argv[1]);
		}
		else
			return CMD_INVALIDPARAM;
		return CMD_OK;
	}
	else
		return CMD_INVALIDPARAM;
}

static CMD_STATUS get_cmd(int argc, const char* argv[])
{
	if (argc == 1)
	{
		if (!strcmp_P(argv[0], PSTR("x")))
		{
			printf_P(PSTR("x is %g\n"), _x);
		}
		else if (!strcmp_P(argv[0], PSTR("y")))
		{
			printf_P(PSTR("y is %g\n"), _y);
		}
		else if (!strcmp_P(argv[0], PSTR("vref")))
		{
			printf_P(PSTR("vref is %g\n"), vref);
		}
		else if (!strcmp_P(argv[0], PSTR("v")))
		{
			printf_P(PSTR("v is %g\n"), v);
		}
		else if (!strcmp_P(argv[0], PSTR("theta")))
		{
			printf_P(PSTR("theta is %g\n"), theta);
		}
		else
			return CMD_INVALIDPARAM;
		return CMD_OK;
	}
	else
		return CMD_INVALIDPARAM;
}

static CMD_STATUS mulxy_cmd(int argc, const char* argv[])
{
	UNUSED(argv);
	if (argc == 0)
	{
		printf_P(PSTR("%g\n"), _x*_y);
		return CMD_OK;
	}
	else
		return CMD_INVALIDPARAM;
}

static CMD_STATUS ctrl_cmd(int argc, const char* argv[])
{
	UNUSED(argv);
	if (argc == 0)
	{
		const float input[] = {vref, v, theta};
		float* y = ctrl_run(input);
		printf_P(PSTR("%g\n"), y[0]);
		return CMD_OK;
	}
	else
		return CMD_INVALIDPARAM;
}
