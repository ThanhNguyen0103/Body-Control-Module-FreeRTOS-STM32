/*
 * adc.c
 *
 *  Created on: Jul 12, 2026
 *      Author: nguyz
 */
#include "adc.h"
extern ADC_HandleTypeDef hadc1;

void ADC_Init(void) {
	HAL_ADC_Start(&hadc1);
}

uint32_t ADC_ReadValue(void) {
	if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
		return HAL_ADC_GetValue(&hadc1);

	}
}

