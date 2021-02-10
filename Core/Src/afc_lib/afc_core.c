/*
 * afc_core.c
 *
 *  Created on: Dec 31, 2020
 *      Author: hendrik
 */


#include "afc_lib/afc_core.h"
#include "main.h"
#include "cmsis_os.h"

void led_task(void* args);


osThreadId_t led_task_handle;
const osThreadAttr_t led_task_attributes = {
  .name = "ledTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 64 * 4
};

led_task_data_t led_task_data;

HAL_StatusTypeDef afc_core_init() {

	led_task_handle = osThreadNew(led_task, NULL, &led_task_attributes);

	return HAL_OK;
}

void afc_core_set_led_freq(AFC_CORE_LED led, uint16_t delay_ms) {
	switch(led) {
	case LED_RED:
		led_task_data.freq_red = delay_ms;
		break;
	case LED_GREEN:
		led_task_data.freq_green = delay_ms;
		break;
	case LED_BLUE:
		led_task_data.freq_blue = delay_ms;
		break;
	}
}


void led_task(void* args) {
	while(1) {
		uint32_t now = osKernelGetTickCount();

		if(led_task_data.freq_red == 0) {
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		} else if(now - led_task_data.last_toggle_red > led_task_data.freq_red) {
			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
			led_task_data.last_toggle_red = now;
		}

		if(led_task_data.freq_green == 0) {
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		} else if(now - led_task_data.last_toggle_green > led_task_data.freq_green) {
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
			led_task_data.last_toggle_green = now;
		}

		if(led_task_data.freq_blue == 0) {
			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
		} else if(now - led_task_data.last_toggle_blue > led_task_data.freq_blue) {
			HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
			led_task_data.last_toggle_blue = now;
		}

		osDelay(1);
	}
}
