#include "unity_fixture.h"
#include "avr/mock_sfr.h"

#include <stdint.h>
#include "task.h"

TEST_GROUP(Task);

TEST_SETUP(Task)
{
    tasks_init(0.001);
}

TEST_TEAR_DOWN(Task)
{
}

TEST(Task, FrequencyCheck1ms)
{
	uint8_t OCR2_reset 		= 0b11110000;
	uint8_t OCR2_clocksel 	= 0b11110000;
	double timer_interval = 0.001;
	uint8_t res = tasks_calculate_counter(&OCR2_reset, &OCR2_clocksel, timer_interval);
	if (res != 255)
	{
		TEST_ASSERT_BITS(0xFF, 0b11100110, OCR2_reset);
		TEST_ASSERT_BITS_MESSAGE(0b00000111, 0b00000100, OCR2_clocksel, "OCR2_clocksel value expected 0x03, got something else");
	}
	else
	{
		TEST_FAIL_MESSAGE("Frequency not possible");
	}
}

TEST(Task, FrequencyCheck10ms)
{
	uint8_t OCR2_reset 		= 0b11110000;
	uint8_t OCR2_clocksel 	= 0b11110000;
	double timer_interval = 0.01;
	uint8_t res = tasks_calculate_counter(&OCR2_reset, &OCR2_clocksel, timer_interval);
	if (res != 255)
	{
		TEST_FAIL_MESSAGE("Frequency not supposed to be possible");
	}
}

TEST(Task, InitNullCallback)
{
<<<<<<< HEAD
    TEST_ASSERT_NULL(task_get_at(0));
=======
    TEST_ASSERT_NULL(tasks_get_at(0));
>>>>>>> TaskScheduler
}

TEST(Task, InitDisabled)
{
    TEST_ASSERT_FALSE(tasks_is_enabled());
}

TEST(Task, Enable)
{
    tasks_disable();
    tasks_enable();
    TEST_ASSERT_TRUE(tasks_is_enabled());
<<<<<<< HEAD
=======
    TEST_ASSERT_BITS(0b00000111, 0b00000100, mock_tccr2);
>>>>>>> TaskScheduler
}

TEST(Task, Disable)
{
    tasks_enable();
    tasks_disable();
    TEST_ASSERT_FALSE(tasks_is_enabled());
<<<<<<< HEAD
=======
    TEST_ASSERT_BITS_LOW(0b00000111, mock_tccr2);
>>>>>>> TaskScheduler
}

static void _dummy(void) { }
static task_s _dummy_task = { 1, &_dummy };

TEST(Task, SetGetCallback)
{
    uint8_t id = tasks_add(&_dummy_task);
    if (id != 255)
    {
    	TEST_ASSERT_EQUAL_PTR(&_dummy_task, tasks_get_at(id));
    }
    else
    {
    	TEST_FAIL();
    }
}

static uint32_t _call_count;
static void _stub(void) { ++_call_count; }
static task_s _stub_task = { 1, &_stub };

TEST(Task, RunCallback)
{
    _call_count = 0;
<<<<<<< HEAD
    task_set(_stub);
=======
    tasks_add(&_stub_task);
    tasks_trigger_isr();
>>>>>>> TaskScheduler
    tasks_run();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, _call_count, "Expected event callback to be called once.");
}

TEST(Task, TriggerIncr)
{
    tasks_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(1, tasks_triggered());
    tasks_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(2, tasks_triggered());
<<<<<<< HEAD
}

TEST(Task, RunClearsTrigger)
{
    task_set(_stub);
    tasks_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(1, tasks_triggered());
    tasks_run();
    TEST_ASSERT_EQUAL_UINT8(0, tasks_triggered());
=======
>>>>>>> TaskScheduler
}

#include "iospy.h"

TEST(Task, RunNull)
{
<<<<<<< HEAD
    task_set(NULL);
    tasks_enable();
    iospy_hook_out();
    tasks_run();
    iospy_unhook_out();
    TEST_ASSERT_FALSE(tasks_is_enabled());
}

static void _stub2(void) { tasks_trigger_isr(); }

TEST(Task, DisableAfterTimeout)
{
    task_set(_stub2);
=======
    tasks_add(NULL);

>>>>>>> TaskScheduler
    iospy_hook_out();
    tasks_run();
    iospy_unhook_out();
    TEST_ASSERT_FALSE(tasks_is_enabled());
}

TEST_GROUP_RUNNER(Task)
{
    RUN_TEST_CASE(Task, InitNullCallback);
    RUN_TEST_CASE(Task, InitDisabled);
    RUN_TEST_CASE(Task, FrequencyCheck1ms);
    RUN_TEST_CASE(Task, FrequencyCheck10ms);
    RUN_TEST_CASE(Task, Enable);
    RUN_TEST_CASE(Task, Disable);
    RUN_TEST_CASE(Task, SetGetCallback);
    RUN_TEST_CASE(Task, RunCallback);
    RUN_TEST_CASE(Task, TriggerIncr);
    RUN_TEST_CASE(Task, RunNull);
<<<<<<< HEAD
    RUN_TEST_CASE(Task, DisableAfterTimeout);
=======
>>>>>>> TaskScheduler
}
