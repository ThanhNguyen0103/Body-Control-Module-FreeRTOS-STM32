/*
 * led.h
 *
 *  Created on: Jul 3, 2026
 *      Author: nguyz
 */

#ifndef DRIVERS_LED_LED_H_
#define DRIVERS_LED_LED_H_

#include "stdbool.h"
#include "stdint.h"
#include "main.h"

typedef struct {
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;

	bool isOn;

	uint32_t lastBlinkTick;
	uint32_t blinkPeriod;

} Led_t;

typedef enum {
	LED_OFF = 0, LED_ON, LED_BLINK
} LedMode_t;

void Led_Init(Led_t *led, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Led_On(Led_t *led);

void Led_Off(Led_t *led);

void Led_Toggle(Led_t *led);

void Led_Blink(Led_t *led);

bool Led_IsOn(Led_t *led);

#endif /* DRIVERS_LED_LED_H_ */
