/*
 * bcm.c
 *
 *  Created on: Oct 4, 2026
 *      Author: nguyz
 */

#include "bcm.h"
#include "main.h"
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "bcm_event.h"
#include "bcm_output.h"
#include "Common/types.h"

#define ADC_MAX_VALUE      4095U
#define ECO_MAX_SPEED      45U
#define SPORT_MAX_SPEED    70U

static void BCMTask(void *argument);
static void BCM_SendOutput(void);

QueueHandle_t bcmEventQueue;
QueueHandle_t bcmOutputQueue;

static IgnitionState_t ignitionState;
static Driving_Mode_t drivingMode;
static uint16_t vehicleSpeed;
static bool brakePressed = false;
static bool cruiseEnabled = false;
static uint16_t cruiseSetSpeed = 0U;

void BCM_Init(void) {
	ignitionState = IGNITION_OFF;
	drivingMode = ECO_MODE;
	vehicleSpeed = 0U;
	brakePressed = false;
	cruiseEnabled = false;
	cruiseSetSpeed = 0U;

	bcmEventQueue = xQueueCreate(10, sizeof(BCM_Event_t));
	bcmOutputQueue = xQueueCreate(10, sizeof(BCM_Output_t));

	if (bcmEventQueue == NULL || bcmOutputQueue == NULL) {
		Error_Handler();
	}

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
	output.cruiseEnabled = cruiseEnabled;

	xQueueSend(bcmOutputQueue, &output, 0);
}

static void BCMTask(void *argument) {
	BCM_Event_t event;

	for (;;) {
		if (xQueueReceive(bcmEventQueue, &event,
		portMAX_DELAY) == pdPASS) {
			switch (event.type) {
			case BCM_EVENT_IGNITION_PRESSED: {
				ignitionState =
						(ignitionState == IGNITION_OFF) ?
								IGNITION_ON : IGNITION_OFF;

				cruiseEnabled = false;
				cruiseSetSpeed = 0U;

				BCM_SendOutput();
				break;
			}

			case BCM_EVENT_DRIVING_MODE_PRESSED: {
				if (ignitionState == IGNITION_ON) {
					drivingMode =
							(drivingMode == ECO_MODE) ? SPORT_MODE : ECO_MODE;

					BCM_SendOutput();
				}

				break;
			}

			case BCM_EVENT_SPEED_INPUT: {
				uint32_t maxSpeed;
				uint32_t targetSpeed;

				if (drivingMode == ECO_MODE) {
					maxSpeed = ECO_MAX_SPEED;
				} else {
					maxSpeed = SPORT_MAX_SPEED;
				}

				if (ignitionState == IGNITION_OFF) {

					cruiseEnabled = false;
					cruiseSetSpeed = 0U;
					targetSpeed = 0U;

				}

				else if (brakePressed) {
					cruiseEnabled = false;
					targetSpeed = 0U;
				} else if (cruiseEnabled) {
					if (cruiseSetSpeed > maxSpeed) {
						cruiseSetSpeed = (uint16_t) maxSpeed;
					}

					targetSpeed = cruiseSetSpeed;
				} else {
					uint32_t adcValue = event.value;

					if (adcValue > ADC_MAX_VALUE) {
						adcValue = ADC_MAX_VALUE;
					}

					targetSpeed = (adcValue * maxSpeed) / ADC_MAX_VALUE;
				}

				if ((uint32_t) vehicleSpeed < targetSpeed) {

					vehicleSpeed++;
				} else if ((uint32_t) vehicleSpeed > targetSpeed) {
					vehicleSpeed--;
				}

				BCM_SendOutput();
				break;
			}

			case BCM_EVENT_BRAKE_PRESSED: {
				brakePressed = true;
				cruiseEnabled = false;
				cruiseSetSpeed = 0U;

				BCM_SendOutput();
				break;
			}

			case BCM_EVENT_BRAKE_RELEASED: {
				brakePressed = false;

				BCM_SendOutput();
				break;
			}

			case BCM_EVENT_CRUISE_PRESSED: {
				if (ignitionState == IGNITION_ON && !brakePressed) {
					if (cruiseEnabled) {

						cruiseEnabled = false;
						cruiseSetSpeed = 0U;

					} else if (vehicleSpeed > 0U) {

						cruiseSetSpeed = vehicleSpeed;
						cruiseEnabled = true;
					}
				}

				BCM_SendOutput();
				break;
			}

			default:
				break;
			}
		}
	}
}
