/*
 * afc_barometer.h
 *
 *  Created on: Dec 31, 2020
 *      Author: hendrik
 */

#ifndef INC_AFC_LIB_AFC_BAROMETER_H_
#define INC_AFC_LIB_AFC_BAROMETER_H_

#include "stm32f4xx_hal.h"


/*!
 *  Struct to hold calibration data.
 */
typedef struct {
  uint16_t dig_T1; /**< dig_T1 cal register. */
  int16_t dig_T2;  /**<  dig_T2 cal register. */
  int16_t dig_T3;  /**< dig_T3 cal register. */

  uint16_t dig_P1; /**< dig_P1 cal register. */
  int16_t dig_P2;  /**< dig_P2 cal register. */
  int16_t dig_P3;  /**< dig_P3 cal register. */
  int16_t dig_P4;  /**< dig_P4 cal register. */
  int16_t dig_P5;  /**< dig_P5 cal register. */
  int16_t dig_P6;  /**< dig_P6 cal register. */
  int16_t dig_P7;  /**< dig_P7 cal register. */
  int16_t dig_P8;  /**< dig_P8 cal register. */
  int16_t dig_P9;  /**< dig_P9 cal register. */
} afc_bar_bmp280_calib_t;


HAL_StatusTypeDef afc_baro_init();

HAL_StatusTypeDef afc_baro_deinit();

void afc_baro_read_calibration_data(afc_bar_bmp280_calib_t *calibration_data);

float afc_baro_read_temperature();

float afc_baro_read_pressure();

#endif /* INC_AFC_LIB_AFC_BAROMETER_H_ */
