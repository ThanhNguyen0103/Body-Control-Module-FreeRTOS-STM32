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
#include "bcm_output.h"

#include "Common/types.h"

static void BCMTask(void *argument);

QueueHandle_t bcmEventQueue;
QueueHandle_t bcmOutputQueue;

static IgnitionState_t ignitionState;
static Driving_Mode_t drivingMode;
static uint16_t vehicleSpeed;

void BCM_Init(void) {

	ignitionState = IGNITION_OFF;
	drivingMode = ECO_MODE;
	vehicleSpeed = 0;

	bcmEventQueue = xQueueCreate(10, sizeof(BCM_Event_t));
	bcmOutputQueue = xQueueCreate(10, sizeof(BCM_Output_t));

	xTaskCreate(BCMTask, "BCMTask", 128,
	NULL, 2,
	NULL);
}

static void BCMTask(void *argument) {

	BCM_Event_t event;
	BCM_Output_t output;

	for (;;) {
		if (xQueueReceive(bcmEventQueue, &event,
		portMAX_DELAY) == pdPASS) {
			switch (event.type) {
			case BCM_EVENT_IGNITION_PRESSED:
				ignitionState =
						(ignitionState == IGNITION_OFF) ?
								IGNITION_ON : IGNITION_OFF;

				if (ignitionState == IGNITION_ON) {
					output.type = BCM_OUTPUT_IGNITION_ON;
				} else {
					output.type = BCM_OUTPUT_IGNITION_OFF;
				}

				xQueueSend(bcmOutputQueue, &output, 0);

				break;

			case BCM_EVENT_DRIVING_MODE_PRESSED:
				if (ignitionState == IGNITION_ON) {
					drivingMode =
							(drivingMode == ECO_MODE) ? SPORT_MODE : ECO_MODE;

					if (drivingMode == ECO_MODE) {
						output.type = BCM_OUTPUT_DRIVING_MODE_ECO;
					} else {
						output.type = BCM_OUTPUT_DRIVING_MODE_SPORT;
					}

					xQueueSend(bcmOutputQueue, &output, 0);
				}
				break;
			case BCM_EVENT_VEHICLE_SPEED:
				vehicleSpeed = event.value;
				break;
			default:
				break;
			}
		}
	}
}
