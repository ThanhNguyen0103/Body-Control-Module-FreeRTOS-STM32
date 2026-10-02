/*
 * led.c
 *
 *  Created on: Jul 3, 2026
 *      Author: nguyz
 */

#include "led.h"
#include "main.h"
void Led_Init(Led_t *led, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	led->GPIOx = GPIOx;
	led->GPIO_Pin = GPIO_Pin;

	led->isOn = false;

	led->lastBlinkTick = 0;
	led->blinkPeriod = 500;

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void Led_On(Led_t *led) {
	HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_SET);

	led->isOn = true;
}

void Led_Off(Led_t *led) {
	HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_RESET);

	led->isOn = false;
}

void Led_Toggle(Led_t *led) {
	HAL_GPIO_TogglePin(led->GPIOx, led->GPIO_Pin);

	led->isOn = !led->isOn;
}

void Led_Blink(Led_t *led) {
	if (HAL_GetTick() - led->lastBlinkTick >= led->blinkPeriod) {
		led->lastBlinkTick = HAL_GetTick();
		Led_Toggle(led);
	}
}

bool Led_IsOn(Led_t *led) {
	return led->isOn;
}

