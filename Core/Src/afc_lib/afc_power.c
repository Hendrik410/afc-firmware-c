/*
 * afc_power.c
 *
 *  Created on: Dec 31, 2020
 *      Author: hendrik
 */

#include "afc_lib/afc_power.h"

#include "stm32f4xx_hal_dma.h"

extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;

// The factor of the voltage divider measuring the input voltage
#define VOLTAGE_DIVIDER_FACTOR 4.3

// The current sensor is bidirectional, so the half the logical supply voltage corresponds with a current of 0 A
#define CURRENT_QUIESCENT_VOLTAGE (3.3 / 2.0)
// The factor to convert the voltage difference to the quiescent voltage into an current in ampere
#define CURRENT_CONVERSION_FACTOR (1.0 / 0.09)

// this memory address holds the calibration value, which is written during production of the mcu
#define ADC_REFERENCE_CAL (*((uint16_t*)0x1FFF7A2A))

afc_pwr_buffer_t afc_pwr_buffer;


/**
 * @brief Calculates the factor by which to multiply the ADC value to correct for deviations in the internal voltage reference.
 */
float calculate_reference_correction_factor();


HAL_StatusTypeDef afc_pwr_init() {
	return HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&afc_pwr_buffer, sizeof(afc_pwr_buffer_t));
}

HAL_StatusTypeDef afc_pwr_deinit() {
	return HAL_ADC_Stop_DMA(&hadc1);
}

float afc_pwr_get_current_consumption() {
	float measured_voltage = (float)afc_pwr_buffer.input_current_raw * calculate_reference_correction_factor() / 4095.0;

	return (measured_voltage - CURRENT_QUIESCENT_VOLTAGE) * CURRENT_CONVERSION_FACTOR;
}

float afc_pwr_get_supply_voltage() {
	return (float) afc_pwr_buffer.input_voltage_raw / 4095.0 * calculate_reference_correction_factor() * VOLTAGE_DIVIDER_FACTOR;
}

float afc_pwr_get_temperature() {
	float measured_voltage = (float)afc_pwr_buffer.onboard_temperature_raw * calculate_reference_correction_factor() / 4095.0;

	return (measured_voltage - 0.76) / 2.5E-3 + 25.0;
}

float calculate_reference_correction_factor() {
	if (afc_pwr_buffer.voltage_reference_measured == 0) return 0;

	return 3.3 * (float)ADC_REFERENCE_CAL / (float)afc_pwr_buffer.voltage_reference_measured;
}


