/*
 * output_task.c
 *
 *  Created on: Oct 5, 2026
 *      Author: nguyz
 */

#include "output_task.h"

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "BCM/bcm.h"
#include "BCM/bcm_output.h"

#include "Drivers/LED/led.h"

static void OutputTask(void *argument);
static Led_t led_ignit;
static Led_t led_driving_mode;
void OutputTask_Init(void) {

	Led_Init(&led_ignit, GPIOA, GPIO_PIN_2);
	Led_Init(&led_driving_mode, GPIOA, GPIO_PIN_1);

	xTaskCreate(OutputTask, "OutputTask", 128,
	NULL, 1,
	NULL);
}
static void OutputTask(void *argument) {
	BCM_Output_t output;

	for (;;) {
		if (xQueueReceive(bcmOutputQueue, &output,
		portMAX_DELAY) == pdPASS) {
			switch (output.type) {
			case BCM_OUTPUT_IGNITION_ON:
				Led_On(&led_ignit);
				break;
			case BCM_OUTPUT_IGNITION_OFF:
				Led_Off(&led_ignit);
				break;
			case BCM_OUTPUT_DRIVING_MODE_ECO:
				Led_On(&led_driving_mode);
				break;
			case BCM_OUTPUT_DRIVING_MODE_SPORT:
				Led_Off(&led_driving_mode);
				break;
			default:

				break;
			}
		}
	}
}

