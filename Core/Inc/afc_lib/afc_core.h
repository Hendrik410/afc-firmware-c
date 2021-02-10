/*
 * afc_core.h
 *
 *  Created on: Dec 31, 2020
 *      Author: hendrik
 */

#ifndef INC_AFC_LIB_AFC_CORE_H_
#define INC_AFC_LIB_AFC_CORE_H_

#include "stm32f4xx_hal.h"

typedef enum {
	LED_RED,
	LED_GREEN,
	LED_BLUE
} AFC_CORE_LED;

typedef struct {
	uint16_t freq_red;
	uint16_t freq_green;
	uint16_t freq_blue;
	uint32_t last_toggle_red;
	uint32_t last_toggle_green;
	uint32_t last_toggle_blue;
} led_task_data_t;

/**
 * @brief Initializes the core module
 */
HAL_StatusTypeDef afc_core_init();

/**
 * @brief Sets the blink state of a led
 * @param led The led that shall be set
 * @param delay_ms The wait time between the toggles for the led. 0 means off
 */
void afc_core_set_led_freq(AFC_CORE_LED led, uint16_t delay_ms);

#endif /* INC_AFC_LIB_AFC_CORE_H_ */
