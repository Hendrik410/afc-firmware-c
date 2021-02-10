/*
 * afc_imu.c
 *
 *  Created on: 4 Jan 2021
 *      Author: hendrik
 */


#include "afc_lib/afc_imu.h"

#define ICM_ADDR ((uint16_t)(0x68 << 1))
#define ICM_WHOAMI            0xEA

 /*
  * ICM20948 uses register banks. Register 127 (0x7F) is used to switch between 4 banks.
  * There's room in the upper address byte below the port speed setting to code in the
  * used bank. This is a bit more efficient, already in use for the speed setting and more
  * in one place than a solution with a lookup table for address/bank pairs.
  */

 #define BANK0   0x0000
 #define BANK1   0x0100
 #define BANK2   0x0200
 #define BANK3   0x0300

 #define BANK_REG_MASK   0x0300
 #define REG_BANK(r)             (((r) & BANK_REG_MASK)>>4)
 #define REG_ADDRESS(r)          ((r) & ~BANK_REG_MASK)

 #define ICMREG_20948_BANK_SEL 0x7F

 #define ICMREG_20948_WHOAMI                 (0x00 | BANK0)
 #define ICMREG_20948_USER_CTRL              (0x03 | BANK0)
 #define ICMREG_20948_PWR_MGMT_1             (0x06 | BANK0)
 #define ICMREG_20948_PWR_MGMT_2             (0x07 | BANK0)
 #define ICMREG_20948_INT_PIN_CFG            (0x0F | BANK0)
 #define ICMREG_20948_INT_ENABLE             (0x10 | BANK0)
 #define ICMREG_20948_INT_ENABLE_1           (0x11 | BANK0)
 #define ICMREG_20948_ACCEL_XOUT_H           (0x2D | BANK0)
 #define ICMREG_20948_INT_ENABLE_2           (0x12 | BANK0)
 #define ICMREG_20948_INT_ENABLE_3           (0x13 | BANK0)
 #define ICMREG_20948_EXT_SLV_SENS_DATA_00   (0x3B | BANK0)
 #define ICMREG_20948_GYRO_SMPLRT_DIV        (0x00 | BANK2)
 #define ICMREG_20948_GYRO_CONFIG_1          (0x01 | BANK2)
 #define ICMREG_20948_GYRO_CONFIG_2          (0x02 | BANK2)
 #define ICMREG_20948_ACCEL_SMPLRT_DIV_1     (0x10 | BANK2)
 #define ICMREG_20948_ACCEL_SMPLRT_DIV_2     (0x11 | BANK2)
 #define ICMREG_20948_ACCEL_CONFIG           (0x14 | BANK2)
 #define ICMREG_20948_ACCEL_CONFIG_2         (0x15 | BANK2)
 #define ICMREG_20948_I2C_MST_CTRL           (0x01 | BANK3)
 #define ICMREG_20948_I2C_SLV0_ADDR          (0x03 | BANK3)
 #define ICMREG_20948_I2C_SLV0_REG           (0x04 | BANK3)
 #define ICMREG_20948_I2C_SLV0_CTRL          (0x05 | BANK3)
 #define ICMREG_20948_I2C_SLV0_DO            (0x06 | BANK3)


extern I2C_HandleTypeDef hi2c1;


HAL_StatusTypeDef afc_imu_init() {
	HAL_StatusTypeDef result;
	uint8_t buf;

	result = HAL_I2C_IsDeviceReady(&hi2c1, ICM_ADDR, 3, 1000);
	if (result != HAL_OK) return result;

	result = HAL_I2C_Mem_Read(&hi2c1, ICM_ADDR, ICMREG_20948_WHOAMI, 1, &buf, 1, 1000);
	if (result != HAL_OK) return result;
	if (buf != ICM_WHOAMI) return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef afc_imu_deinit() {

}
