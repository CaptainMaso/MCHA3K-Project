#include "unity_fixture.h"

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

TEST(Task, InitNullCallback)
{
    TEST_ASSERT_NULL(task_get_at(0));
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
}

TEST(Task, Disable)
{
    tasks_enable();
    tasks_disable();
    TEST_ASSERT_FALSE(tasks_is_enabled());
}

static void _dummy(void) { }

TEST(Task, SetGetCallback)
{
    task_set(_dummy);
    TEST_ASSERT_EQUAL_PTR(_dummy, task_get());
}

static uint32_t _call_count;
static void _stub(void) { ++_call_count; }

TEST(Task, RunCallback)
{
    _call_count = 0;
    task_set(_stub);
    tasks_run();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, _call_count, "Expected event callback to be called once.");
}

TEST(Task, TriggerIncr)
{
    tasks_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(1, tasks_triggered());
    tasks_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(2, tasks_triggered());
}

TEST(Task, RunClearsTrigger)
{
    task_set(_stub);
    tasks_trigger_isr();
    TEST_ASSERT_EQUAL_UINT8(1, tasks_triggered());
    tasks_run();
    TEST_ASSERT_EQUAL_UINT8(0, tasks_triggered());
}

#include "iospy.h"

TEST(Task, RunNull)
{
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
    iospy_hook_out();
    tasks_run();
    iospy_unhook_out();
    TEST_ASSERT_FALSE(tasks_is_enabled());
}

TEST_GROUP_RUNNER(Task)
{
    RUN_TEST_CASE(Task, InitNullCallback);
    RUN_TEST_CASE(Task, InitDisabled);
    RUN_TEST_CASE(Task, Enable);
    RUN_TEST_CASE(Task, Disable);
    RUN_TEST_CASE(Task, SetGetCallback);
    RUN_TEST_CASE(Task, RunCallback);
    RUN_TEST_CASE(Task, TriggerIncr);
    RUN_TEST_CASE(Task, RunClearsTrigger);
    RUN_TEST_CASE(Task, RunNull);
    RUN_TEST_CASE(Task, DisableAfterTimeout);
}
