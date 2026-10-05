/*
 * input.c
 *
 *  Created on: Sep 29, 2026
 *      Author: nguyz
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "main.h"
#include "button.h"

#include "BCM/bcm_event.h"
#include "BCM/bcm.h"
#include "input_task.h"

static Button_t button_ignition;
static Button_t button_driving_mode;

void InputTask_Init(void) {

	Button_Init(&button_ignition,
	GPIOB,
	GPIO_PIN_12);
	Button_Init(&button_driving_mode, GPIOB, GPIO_PIN_13);

	xTaskCreate(InputTask, "InputTask", 128,
	NULL, 2,
	NULL);
}

void InputTask(void *argument) {
	for (;;) {

		Button_Update(&button_ignition);
		Button_Update(&button_driving_mode);

		if (Button_IsPressed(&button_ignition)) {

			BCM_Event_t event;
			event.type = BCM_EVENT_IGNITION_PRESSED;

			xQueueSend(bcmEventQueue, &event, 0);
		}
		if (Button_IsPressed(&button_driving_mode)) {
			BCM_Event_t event;
			event.type = BCM_EVENT_DRIVING_MODE_PRESSED;

			xQueueSend(bcmEventQueue, &event, 0);
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

