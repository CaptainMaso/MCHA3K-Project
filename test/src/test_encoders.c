#include "encoders.h"
#include "unity_fixture.h"

#include <stdint.h>
#include "avr/io.h"
#include "avr/mock_sfr.h"

TEST_GROUP(Encoders);

TEST_SETUP(Encoders)
{
    encoder_init();
    PORTD = 0x00;
    PORTB = 0x00;
}

TEST_TEAR_DOWN(Encoders)
{
}

TEST(Encoders, DefaultCount)
{
    TEST_ASSERT_EQUAL_INT32(0, encoder_get_count(MOTOR_LEFT));
    TEST_ASSERT_EQUAL_INT32(0, encoder_get_count(MOTOR_RIGHT));
}

TEST(Encoders, SetCount)
{
    encoder_set_count(42, MOTOR_LEFT);
    TEST_ASSERT_EQUAL_INT32(42, encoder_get_count(MOTOR_LEFT));
    encoder_set_count(86, MOTOR_RIGHT);
    TEST_ASSERT_EQUAL_INT32(86, encoder_get_count(MOTOR_RIGHT));
}

static void raise_MLA(void) { PORTD |=   _BV(PD3); }  // Raise PD2/INT0/chA
static void raise_MLB(void) { PORTB |=   _BV(PB1); }  // Raise PD3/INT1/chB
static void lower_MLA(void) { PORTD &=   ~(_BV(PD3)); }  // Lower PD2/INT0/chA
static void lower_MLB(void) { PORTB &=   ~(_BV(PB1)); }  // Lower PD3/INT1/chB

static void raise_MRA(void) { PORTD |=   _BV(PD2); }  // Raise PD2/INT0/chA
static void raise_MRB(void) { PORTB |=   _BV(PB0); }  // Raise PD3/INT1/chB
static void lower_MRA(void) { PORTD &=   ~(_BV(PD2)); }  // Lower PD2/INT0/chA
static void lower_MRB(void) { PORTB &=   ~(_BV(PB0)); }  // Lower PD3/INT1/chB

TEST(Encoders, DualIncrement)
{
	//Motor Left
    PORTD = 0x00;
    PORTB = 0x00;
    encoder_set_count(0, MOTOR_LEFT);

    for (int i = 0; i < 10; i++)
    {
    	// Transition from state 1 to 4
		raise_MLA();
		encoder_edge_ML_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(2*i+1, encoder_get_count(MOTOR_LEFT), "Expected increment for state 1 to 2 transition: ML");

		// Transition from state 2 to 3
		raise_MLB();

		// Transition from state 3 to 4
		lower_MLA();
		encoder_edge_ML_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(2*i+2, encoder_get_count(MOTOR_LEFT), "Expected increment for state 3 to 4 transition: ML");

		// Transition from state 4 to 1
		lower_MLB();
    }

    //Motor Right
    PORTD = 0x00;
    PORTB = 0x00;
	encoder_set_count(0, MOTOR_RIGHT);

	for (int i = 0; i < 10; i++)
	{
		// Transition from state 1 to 2
		lower_MRA();
		encoder_edge_MR_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(2*i+1, encoder_get_count(MOTOR_RIGHT), "Expected increment for state 1 to 2 transition: MR");

		// Transition from state 2 to 3
		raise_MRB();

		// Transition from state 3 to 4
		raise_MRA();
		encoder_edge_MR_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(2*i+2, encoder_get_count(MOTOR_RIGHT), "Expected increment for state 3 to 4 transition: MR");

		// Transition from state 4 to 1
		lower_MRB();
	}
}

TEST(Encoders, DualDecrement)
{
	//Motor Left
	encoder_set_count(0, MOTOR_LEFT);

	for (int i = 0; i < 10; i++)
	{
		// Transition from state 1 to 2
		lower_MLA();
		encoder_edge_ML_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(-(2*i+1), encoder_get_count(MOTOR_LEFT), "Expected increment for state 1 to 2 transition: ML");

		// Transition from state 2 to 3
		raise_MLB();

		// Transition from state 3 to 4
		raise_MLA();
		encoder_edge_ML_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(-(2*i+2), encoder_get_count(MOTOR_LEFT), "Expected increment for state 3 to 4 transition: ML");

		// Transition from state 4 to 1
		lower_MLB();
	}

	//Motor Right
	encoder_set_count(0, MOTOR_RIGHT);

	for (int i = 0; i < 10; i++)
	{
		// Transition from state 1 to 4
		raise_MRA();
		encoder_edge_MR_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(-(2*i+1), encoder_get_count(MOTOR_RIGHT), "Expected increment for state 1 to 2 transition: MR");

		// Transition from state 2 to 3
		raise_MRB();

		// Transition from state 3 to 4
		lower_MRA();
		encoder_edge_MR_isr();
		TEST_ASSERT_EQUAL_INT32_MESSAGE(-(2*i+2), encoder_get_count(MOTOR_RIGHT), "Expected increment for state 3 to 4 transition: MR");

		// Transition from state 4 to 1
		lower_MRB();
	}
}

TEST(Encoders, PopCount)
{
    encoder_set_count(123, MOTOR_LEFT);
    TEST_ASSERT_EQUAL_INT32(123, encoder_pop_count(MOTOR_LEFT));
    TEST_ASSERT_EQUAL_INT32(0, encoder_get_count(MOTOR_LEFT));

    encoder_set_count(523, MOTOR_RIGHT);
	TEST_ASSERT_EQUAL_INT32(523, encoder_pop_count(MOTOR_RIGHT));
	TEST_ASSERT_EQUAL_INT32(0, encoder_get_count(MOTOR_RIGHT));
}

TEST_GROUP_RUNNER(Encoders)
{
    RUN_TEST_CASE(Encoders, DefaultCount);
    RUN_TEST_CASE(Encoders, SetCount);
    RUN_TEST_CASE(Encoders, DualIncrement);
    RUN_TEST_CASE(Encoders, DualDecrement);
    RUN_TEST_CASE(Encoders, PopCount);
}
