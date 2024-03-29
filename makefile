# We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
    TARGET_TEST_EXTENSION=.exe
    LD_WRAP=true
else
    TARGET_TEST_EXTENSION=.out
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LD_WRAP=true
    endif
    ifeq ($(UNAME_S),Darwin)
        LD_WRAP=false
    endif
endif

F_CPU = 14745600

UNITY_ROOT = Unity

TARGET_BASE = Project
TARGET_ELF = bin/$(TARGET_BASE).elf
TARGET_HEX = bin/$(TARGET_BASE).hex

TARGET_TEST_BASE = test_$(TARGET_BASE)
TARGET_TEST = test/bin/$(TARGET_TEST_BASE)$(TARGET_TEST_EXTENSION)

COMMONDEFS = -DF_CPU=$(F_CPU)UL

CDEFS = $(COMMONDEFS)

CFLAGS  = -std=c99
CFLAGS += $(CDEFS)
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -Wpointer-to-int-cast
CFLAGS += -Wcomment
CFLAGS += -Wtype-limits

# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# PRINTF_LIB =
# PRINTF_LIB = $(PRINTF_LIB_MIN)
PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# SCANF_LIB =
# SCANF_LIB = $(SCANF_LIB_MIN)
SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

# AVR Linker flags
LDFLAGS_AVR = $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

SRC_COMMON = \
	src/circ_buffer.c \
	src/cmd_line_buffer.c \
	src/cmd_parser.c \
	src/task.c \
	src/log_data.c \
	src/encoders.c \
	src/sysid.c \
	src/kf.c \
	src/mpu6050.c \
	src/twimaster.c \
	src/controller.c \
	src/pend_ctrl.c \
	src/motor.c
	
SRC_TEST = \
	$(UNITY_ROOT)/src/unity.c \
	$(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
	$(SRC_COMMON) \
	mock/avr/mock_sfr.c \
	test/src/iospy.c \
	test/src/all_tests.c \
	test/src/test_circ_buffer.c \
	test/src/test_cmd_line_buffer.c \
	test/src/test_iospy.c \
	test/src/test_cmd_parse.c \
	test/src/test_cmd_process.c \
	test/src/test_task.c \
	test/src/test_encoders.c \
	test/src/test_motor.c

SRC_AVR = \
	$(SRC_COMMON) \
	src/main.c \
	src/uart.c \
	src/uart_isr.c\
	src/task_isr.c \
	src/encoders_isr.c \
	src/controller_isr.c \
	src/motor_isr.c 

INC_COMMON = \
	-Isrc

INC_TEST = \
	-I$(UNITY_ROOT)/src \
	-I$(UNITY_ROOT)/extras/fixture/src \
	$(INC_COMMON) \
	-Imock

INC_AVR = \
	$(INC_COMMON)
	
DTREXE = \
	dtr.exe
	
PORT = \
	COM1

SYMBOLS =

MISC_TEST = -DUNITY_FLOAT_PRECISION=0.0001f
ifeq ($(LD_WRAP),true)
	MISC_TEST += -Wl,-wrap,cmd_parse
else
	MISC_TEST += -DNO_LD_WRAP
endif

clean_avr:
	rm -f $(TARGET_ELF) $(TARGET_HEX)

avr: clean_avr
	avr-gcc -g -Os -mmcu=atmega32 $(CFLAGS) $(INC_AVR) $(SRC_AVR) -o $(TARGET_ELF) $(LDFLAGS_AVR)
	avr-size $(TARGET_ELF)
	avr-objcopy -j .text -j .data -O ihex $(TARGET_ELF) $(TARGET_HEX)
	
reboot:
	./$(DTREXE) $(PORT) LOW 8
	
program: reboot
	avrdude -p atmega32 -c avr109 -P $(PORT) -b 115200 -u -U flash:w:$(TARGET_HEX)
	
	
avr_only: avr program

test: clean_test
	gcc $(CFLAGS) $(INC_TEST) $(SYMBOLS) $(SRC_TEST) -o $(TARGET_TEST) $(MISC_TEST)
	- ./$(TARGET_TEST)

clean_test:
	rm -f $(TARGET_TEST)

clean: clean_test clean_avr
	
default:
	gcc $(CFLAGS) $(INC_TEST) $(SYMBOLS) $(SRC_TEST) -o $(TARGET_TEST) $(MISC_TEST)
	- ./$(TARGET_TEST)
	make avr_only
	
all: clean default
