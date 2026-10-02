/*
 * button.c
 *
 *  Created on: Jul 3, 2026
 *      Author: nguyz
 */

#include "button.h"

void Button_Init(Button_t *button, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {

	button->GPIO_Pin = GPIO_Pin;
	button->GPIOx = GPIOx;
	button->lastState = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

	button->pressed = false;
	button->released = false;
	button->holding = false;

	button->lastTick = HAL_GetTick();
}

void Button_Update(Button_t *button) {
	GPIO_PinState currentState = HAL_GPIO_ReadPin(button->GPIOx,
			button->GPIO_Pin);

	button->pressed = false;
	button->released = false;

	if (currentState != button->lastState) {
		if (HAL_GetTick() - button->lastTick >= 20) {
			button->lastTick = HAL_GetTick();

			button->lastState = currentState;

			if (currentState == GPIO_PIN_RESET) {
				button->pressed = true;
				button->holding = true;
			} else {
				button->released = true;
				button->holding = false;
			}
		}
	}
}

bool Button_IsPressed(Button_t *button) {
	return button->pressed;
}
bool Button_IsReleased(Button_t *button) {
	return button->released;
}
bool Button_IsHolding(Button_t *button) {
	return button->holding;
}

