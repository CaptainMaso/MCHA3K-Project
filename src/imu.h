#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>
#include "i2cmaster.h"

#define IMU_ADDRESS             (0x68) // 0x69 when AD0 pin to Vcc

#define IMU_REG_ACCEL_XOFFS_H     (0x06)
#define IMU_REG_ACCEL_XOFFS_L     (0x07)
#define IMU_REG_ACCEL_YOFFS_H     (0x08)
#define IMU_REG_ACCEL_YOFFS_L     (0x09)
#define IMU_REG_ACCEL_ZOFFS_H     (0x0A)
#define IMU_REG_ACCEL_ZOFFS_L     (0x0B)
#define IMU_REG_GYRO_XOFFS_H      (0x13)
#define IMU_REG_GYRO_XOFFS_L      (0x14)
#define IMU_REG_GYRO_YOFFS_H      (0x15)
#define IMU_REG_GYRO_YOFFS_L      (0x16)
#define IMU_REG_GYRO_ZOFFS_H      (0x17)
#define IMU_REG_GYRO_ZOFFS_L      (0x18)
#define IMU_REG_CONFIG            (0x1A)
#define IMU_REG_GYRO_CONFIG       (0x1B) // Gyroscope Configuration
#define IMU_REG_ACCEL_CONFIG      (0x1C) // Accelerometer Configuration
#define IMU_REG_FF_THRESHOLD      (0x1D)
#define IMU_REG_FF_DURATION       (0x1E)
#define IMU_REG_MOT_THRESHOLD     (0x1F)
#define IMU_REG_MOT_DURATION      (0x20)
#define IMU_REG_ZMOT_THRESHOLD    (0x21)
#define IMU_REG_ZMOT_DURATION     (0x22)
#define IMU_REG_INT_PIN_CFG       (0x37) // INT Pin. Bypass Enable Configuration
#define IMU_REG_INT_ENABLE        (0x38) // INT Enable
#define IMU_REG_INT_STATUS        (0x3A)
#define IMU_REG_ACCEL_XOUT_H      (0x3B)
#define IMU_REG_ACCEL_XOUT_L      (0x3C)
#define IMU_REG_ACCEL_YOUT_H      (0x3D)
#define IMU_REG_ACCEL_YOUT_L      (0x3E)
#define IMU_REG_ACCEL_ZOUT_H      (0x3F)
#define IMU_REG_ACCEL_ZOUT_L      (0x40)
#define IMU_REG_TEMP_OUT_H        (0x41)
#define IMU_REG_TEMP_OUT_L        (0x42)
#define IMU_REG_GYRO_XOUT_H       (0x43)
#define IMU_REG_GYRO_XOUT_L       (0x44)
#define IMU_REG_GYRO_YOUT_H       (0x45)
#define IMU_REG_GYRO_YOUT_L       (0x46)
#define IMU_REG_GYRO_ZOUT_H       (0x47)
#define IMU_REG_GYRO_ZOUT_L       (0x48)
#define IMU_REG_MOT_DETECT_STATUS (0x61)
#define IMU_REG_MOT_DETECT_CTRL   (0x69)
#define IMU_REG_USER_CTRL         (0x6A) // User Control
#define IMU_REG_PWR_MGMT_1        (0x6B) // Power Management 1
#define IMU_REG_WHO_AM_I          (0x75) // Who Am I

#define IMU_MAX_DATA_READ 10

typedef enum
{
    IMU_CLOCK_KEEP_RESET      = 0b111,
    IMU_CLOCK_EXTERNAL_19MHZ  = 0b101,
    IMU_CLOCK_EXTERNAL_32KHZ  = 0b100,
    IMU_CLOCK_PLL_ZGYRO       = 0b011,
    IMU_CLOCK_PLL_YGYRO       = 0b010,
    IMU_CLOCK_PLL_XGYRO       = 0b001,
    IMU_CLOCK_INTERNAL_8MHZ   = 0b000
} IMU_clockSource_t;

typedef enum
{
    IMU_SCALE_2000DPS         = 0b11,
    IMU_SCALE_1000DPS         = 0b10,
    IMU_SCALE_500DPS          = 0b01,
    IMU_SCALE_250DPS          = 0b00
} IMU_dps_t;

typedef enum
{
    IMU_RANGE_16G             = 0b11,
    IMU_RANGE_8G              = 0b10,
    IMU_RANGE_4G              = 0b01,
    IMU_RANGE_2G              = 0b00,
} IMU_range_t;

typedef enum
{
    IMU_DELAY_3MS             = 0b11,
    IMU_DELAY_2MS             = 0b10,
    IMU_DELAY_1MS             = 0b01,
    IMU_NO_DELAY              = 0b00,
} IMU_onDelay_t;

typedef enum
{
    IMU_DHPF_HOLD             = 0b111,
    IMU_DHPF_0_63HZ           = 0b100,
    IMU_DHPF_1_25HZ           = 0b011,
    IMU_DHPF_2_5HZ            = 0b010,
    IMU_DHPF_5HZ              = 0b001,
    IMU_DHPF_RESET            = 0b000,
} IMU_dhpf_t;

typedef enum
{
    IMU_DLPF_6                = 0b110,
    IMU_DLPF_5                = 0b101,
    IMU_DLPF_4                = 0b100,
    IMU_DLPF_3                = 0b011,
    IMU_DLPF_2                = 0b010,
    IMU_DLPF_1                = 0b001,
    IMU_DLPF_0                = 0b000,
} IMU_dlpf_t;

void imu_init(void);

void imu_whoami(void);

uint8_t imu_read_reg8(uint8_t);
uint16_t imu_read_reg16(uint8_t);

#endif
