/*
 * bcm.c
 *
 *  Created on: Oct 4, 2026
 *      Author: nguyz
 */

#include "bcm.h"
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "bcm_event.h"

static void BCMTask(void *argument);
QueueHandle_t bcmEventQueue;

void BCM_Init(void) {

	bcmEventQueue = xQueueCreate(10, sizeof(BCM_Event_t));

	xTaskCreate(BCMTask, "BCMTask", 128,
	NULL, 2,
	NULL);
}

static void BCMTask(void *argument) {

	BCM_Event_t event;

	for (;;) {
		if (xQueueReceive(bcmEventQueue, &event,
		portMAX_DELAY) == pdPASS) {
			switch (event.type) {
			case BCM_EVENT_IGNITION_PRESSED:

				break;

			case BCM_EVENT_DRIVING_MODE_PRESSED:

				break;

			default:
				break;
			}
		}
	}
}
