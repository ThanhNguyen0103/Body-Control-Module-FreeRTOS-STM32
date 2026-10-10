/*
 * bcm.c
 *
 *  Created on: Oct 4, 2026
 *      Author: nguyz
 */

#include "bcm.h"
#include "main.h"
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "bcm_event.h"
#include "bcm_output.h"

#include "Common/types.h"

#define ADC_MAX_VALUE      4095
#define ECO_MAX_SPEED      45
#define SPORT_MAX_SPEED    70

static void BCMTask(void *argument);
static void BCM_SendOutput(void);

QueueHandle_t bcmEventQueue;
QueueHandle_t bcmOutputQueue;

static IgnitionState_t ignitionState;
static Driving_Mode_t drivingMode;
static uint16_t vehicleSpeed;
static bool brakePressed = false;

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

static void BCM_SendOutput(void) {
	BCM_Output_t output;

	output.ignition = ignitionState;
	output.mode = drivingMode;
	output.speed = vehicleSpeed;
	output.brakePressed = brakePressed;

	xQueueSend(bcmOutputQueue, &output, 0);
}

static void BCMTask(void *argument) {
	BCM_Event_t event;

	for (;;) {
		if (xQueueReceive(bcmEventQueue, &event,
		portMAX_DELAY) == pdPASS) {
			switch (event.type) {
			case BCM_EVENT_IGNITION_PRESSED:

				ignitionState =
						(ignitionState == IGNITION_OFF) ?
								IGNITION_ON : IGNITION_OFF;

				BCM_SendOutput();

				break;

			case BCM_EVENT_DRIVING_MODE_PRESSED:

				if (ignitionState == IGNITION_ON) {
					drivingMode =
							(drivingMode == ECO_MODE) ? SPORT_MODE : ECO_MODE;

					BCM_SendOutput();
				}

				break;

			case BCM_EVENT_SPEED_INPUT: {
				uint32_t targetSpeed;
				uint32_t maxSpeed;

				if (ignitionState == IGNITION_OFF) {
					if (vehicleSpeed > 0) {
						vehicleSpeed--;
					}

					BCM_SendOutput();

					break;
				}

				if (drivingMode == ECO_MODE) {
					maxSpeed = ECO_MAX_SPEED;
				} else {
					maxSpeed = SPORT_MAX_SPEED;
				}

				if (brakePressed) {
					if (vehicleSpeed > 0) {
						vehicleSpeed--;
					}
				} else {

					targetSpeed = ((uint32_t) event.value * maxSpeed)
							/ ADC_MAX_VALUE;
					if (vehicleSpeed < targetSpeed) {
						vehicleSpeed++;
					} else if (vehicleSpeed > targetSpeed) {
						vehicleSpeed--;
					}
				}

				BCM_SendOutput();

				break;
			}
			case BCM_EVENT_BRAKE_PRESSED:
				brakePressed = true;
				BCM_SendOutput();
				break;

			case BCM_EVENT_BRAKE_RELEASED:
				brakePressed = false;
				BCM_SendOutput();
				break;
			default:
				break;
			}
		}
	}
}
