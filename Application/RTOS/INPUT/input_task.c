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

void InputTask(void *argument) {
	for (;;) {

		Button_Update(&button_ignition);

		if (Button_IsPressed(&button_ignition)) {

			BCM_Event_t event;
			event.type = BCM_EVENT_IGNITION_PRESSED;

			xQueueSend(bcmEventQueue, &event, 0);
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
void InputTask_Init(void) {

	Button_Init(&button_ignition,
	GPIOB,
	GPIO_PIN_12);

	xTaskCreate(InputTask, "InputTask", 128,
	NULL, 2,
	NULL);
}
