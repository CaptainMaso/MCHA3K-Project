#include "unity_fixture.h"

#include <stdint.h>
#include "avr/io.h"
#include "avr/mock_sfr.h"

#include "motor.h"

TEST_GROUP(Motor);

TEST_SETUP(Motor)
{
	motors_init();
}
TEST_TEAR_DOWN(Motor)
{
	OCR1A = 0x00;
	OCR1B = 0x00;

	PORTB = 0x00;

	ADCW = 0x00;
	ADCSRA &= ~(_BV(ADIF));
}

TEST(Motor, SetPWM_Positive)
{
	motors_set_pwm(MOTOR_LEFT, (int32_t)40000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)40000, OCR1A);
	TEST_ASSERT_BITS_LOW(_BV(PB3), PORTB);

	motors_set_pwm(MOTOR_RIGHT, (int32_t)2000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)2000, OCR1B);
	TEST_ASSERT_BITS_HIGH(_BV(PB2), PORTB);
}

TEST(Motor, SetPWM_Negative)
{
	motors_set_pwm(MOTOR_LEFT, (int32_t)-40000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)40000, OCR1A);
	TEST_ASSERT_BITS_HIGH(_BV(PB3), PORTB);

	motors_set_pwm(MOTOR_RIGHT, (int32_t)-2000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)2000, OCR1B);
	TEST_ASSERT_BITS_LOW(_BV(PB2), PORTB);
}

TEST(Motor, SetPWM_Positive_OutOfRange)
{
	motors_set_pwm(MOTOR_LEFT, (int32_t)100000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)0, OCR1A);
	TEST_ASSERT_BITS_LOW(_BV(PB3), PORTB);

	motors_set_pwm(MOTOR_RIGHT, (int32_t)100000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)0, OCR1B);
	TEST_ASSERT_BITS_LOW(_BV(PB2), PORTB);
}

TEST(Motor, SetPWM_Negative_OutOfRange)
{
	motors_set_pwm(MOTOR_LEFT, (int32_t)-100000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)0, OCR1A);
	TEST_ASSERT_BITS_LOW(_BV(PB3), PORTB);

	motors_set_pwm(MOTOR_RIGHT, (int32_t)-100000);
	TEST_ASSERT_EQUAL_UINT16((uint16_t)0, OCR1B);
	TEST_ASSERT_BITS_LOW(_BV(PB2), PORTB);
}

TEST_GROUP_RUNNER(Motor)
{
	RUN_TEST_CASE(Motor, SetPWM_Positive);
	RUN_TEST_CASE(Motor, SetPWM_Negative);
	RUN_TEST_CASE(Motor, SetPWM_Positive_OutOfRange);
	RUN_TEST_CASE(Motor, SetPWM_Negative_OutOfRange);
}
