/*
 * button.h
 *
 *  Created on: Jul 3, 2026
 *      Author: nguyz
 */

#ifndef DRIVERS_BUTTON_BUTTON_H_
#define DRIVERS_BUTTON_BUTTON_H_

#include <stdbool.h>
#include "main.h"

typedef struct {
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;

	GPIO_PinState lastState;
	uint32_t lastTick;

	bool pressed;
	bool released;
	bool holding;

} Button_t;

void Button_Init(Button_t *button, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Button_Update(Button_t *button);

bool Button_IsReleased(Button_t *button);
bool Button_IsPressed(Button_t *button);
bool Button_IsHolding(Button_t *button);

#endif /* DRIVERS_BUTTON_BUTTON_H_ */
