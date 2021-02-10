/*
 * afc_imu.h
 *
 *  Created on: 4 Jan 2021
 *      Author: hendrik
 */

#ifndef INC_AFC_LIB_AFC_IMU_H_
#define INC_AFC_LIB_AFC_IMU_H_

#include "stm32f4xx_hal.h"


HAL_StatusTypeDef afc_imu_init();

HAL_StatusTypeDef afc_imu_deinit();

#endif /* INC_AFC_LIB_AFC_IMU_H_ */
