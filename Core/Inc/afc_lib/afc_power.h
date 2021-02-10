/*
 * afc_power.h
 *
 *  Created on: Dec 31, 2020
 *      Author: hendrik
 */

#ifndef INC_AFC_POWER_H_
#define INC_AFC_POWER_H_

#include "stm32f4xx_hal.h"


typedef struct {
	uint32_t input_voltage_raw;
	uint32_t input_current_raw;
	uint32_t onboard_temperature_raw;
	uint32_t voltage_reference_measured;
} afc_pwr_buffer_t;


/**
 * @brief Initializes the Power Module
 */
HAL_StatusTypeDef afc_pwr_init();

/**
 * @brief Deinitializes the Power Module
 */
HAL_StatusTypeDef afc_pwr_deinit();

/**
 * @brief Calculates and returns the current consumption of the board
 * @retval The current consumption in Ampere
 */
float afc_pwr_get_current_consumption();

/**
 * @brief Calculates and returns the supply voltage of the board
 * @retval The supply volrage in Volt
 */
float afc_pwr_get_supply_voltage();

/**
 * @brief Calculates and returns the on-chip temperature
 * @retval The on-chip temperature
 */
float afc_pwr_get_temperature();


#endif /* INC_AFC_POWER_H_ */
