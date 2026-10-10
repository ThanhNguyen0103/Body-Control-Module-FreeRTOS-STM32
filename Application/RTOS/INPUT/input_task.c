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
static Button_t button_brake;

static void Input_SendEvent(BCM_EventType_t type);

void InputTask_Init(void) {

	Button_Init(&button_ignition,
	GPIOB,
	GPIO_PIN_12);

	Button_Init(&button_driving_mode, GPIOB, GPIO_PIN_13);

	Button_Init(&button_brake, GPIOB, GPIO_PIN_14);

	xTaskCreate(InputTask, "InputTask", 128,
	NULL, 2,
	NULL);
}

static void Input_SendEvent(BCM_EventType_t type) {
	BCM_Event_t event;

	event.type = type;
	event.value = 0;

	(void) xQueueSend(bcmEventQueue, &event, 0);
}
void InputTask(void *argument) {

	(void) argument;

	for (;;) {

		Button_Update(&button_ignition);
		Button_Update(&button_driving_mode);
		Button_Update(&button_brake);

		if (Button_IsPressed(&button_ignition)) {

			Input_SendEvent(BCM_EVENT_IGNITION_PRESSED);
		}
		if (Button_IsPressed(&button_driving_mode)) {
			Input_SendEvent(BCM_EVENT_DRIVING_MODE_PRESSED);
		}

		if (Button_IsPressed(&button_brake)) {
			Input_SendEvent(BCM_EVENT_BRAKE_PRESSED);
		}

		if (Button_IsReleased(&button_brake)) {
			Input_SendEvent(BCM_EVENT_BRAKE_RELEASED);
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

