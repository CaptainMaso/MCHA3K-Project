#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "imu.h"

void imu_init(void)
{

}

void imu_whoami(void)
{
	uint8_t whoami = imu_read_reg8(IMU_REG_WHO_AM_I);
	if (whoami == IMU_ADDRESS)
			printf_P(PSTR("I AM THE MPU6050\n"));
		else
			printf_P(PSTR("I AM NOT THE MPU6050\n"));
}


uint8_t imu_read_reg8(uint8_t reg)
{
	uint8_t data;
	printf_P(PSTR("Sending Start\n"));
	i2c_start_wait(0b1000000 | IMU_ADDRESS);
	printf_P(PSTR("Writing Desired Register\n"));
	i2c_write(reg);
	printf_P(PSTR("Sending repeated start\n"));
	i2c_rep_start(IMU_ADDRESS | 0x00);
	printf_P(PSTR("Reading and returning Nack\n"));
	data = i2c_readNak();
	printf_P(PSTR("Sending stop\n"));
	i2c_stop();
	printf_P(PSTR("Done\n"));
	return data;
}

uint16_t imu_read_reg16(uint8_t reg)
{
	uint8_t dataH;
	uint8_t dataL;
	i2c_start_wait(IMU_ADDRESS | 0x00);
	i2c_write(reg);
	i2c_rep_start(IMU_ADDRESS | 0x01);
	dataH = i2c_readAck();
	dataL = i2c_readNak();
	i2c_stop();
	return (uint16_t)((dataH << 8) | dataL);
}

uint8_t imu_write_reg8(uint8_t reg, uint8_t data)
{
	i2c_start_wait(IMU_ADDRESS | 0x00);
	i2c_write(reg);
	i2c_write(data);
	i2c_stop();
	return 0;
}

uint8_t imu_write_reg16(uint8_t reg, uint16_t data)
{
	uint8_t dataH = (uint8_t)((data & 0xFF00) >> 8);
	uint8_t dataL = (uint8_t)(data & 0x00FF);
	i2c_start_wait(IMU_ADDRESS | 0x00);
	i2c_write(reg);
	i2c_write(dataH);
	i2c_write(dataL);
	i2c_stop();
	return 0;
}
