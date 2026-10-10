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
#include "Drivers/CAN/can.h"

static void OutputTask(void *argument);
static void Output_SendCAN(const BCM_Output_t *output);

static Led_t led_ignition;
static Led_t led_driving_mode;
static Led_t led_brake;

void OutputTask_Init(void) {
	Led_Init(&led_ignition,
	GPIOA,
	GPIO_PIN_2);

	Led_Init(&led_driving_mode,
	GPIOA,
	GPIO_PIN_1);

	Led_Init(&led_brake,
	GPIOA,
	GPIO_PIN_3);

	xTaskCreate(OutputTask, "OutputTask", 128,
	NULL, 1,
	NULL);
}

static void Output_SendCAN(const BCM_Output_t *output) {
	CAN_Frame_t frame;

	frame.id = CAN_ID_BCM_STATUS;
	frame.dlc = 6;

	frame.data[0] = (uint8_t) output->ignition;
	frame.data[1] = (uint8_t) output->mode;

	frame.data[2] = (uint8_t) (output->speed & 0xFF);

	frame.data[3] = (uint8_t) ((output->speed >> 8) & 0xFF);

	frame.data[4] = output->brakePressed ? 1U : 0U;

	frame.data[5] = output->cruiseEnabled ? 1U : 0U;

	CAN_Send(&frame);
}

static void OutputTask(void *argument) {
	BCM_Output_t output;

	for (;;) {
		if (xQueueReceive(bcmOutputQueue, &output,
		portMAX_DELAY) == pdPASS) {

			if (output.ignition == IGNITION_ON) {
				Led_On(&led_ignition);
			} else {
				Led_Off(&led_ignition);
			}

			if (output.mode == ECO_MODE) {
				Led_On(&led_driving_mode);
			} else {
				Led_Off(&led_driving_mode);
			}

			if (output.brakePressed) {
				Led_On(&led_brake);
			} else {
				Led_Off(&led_brake);
			}

			Output_SendCAN(&output);
		}
	}
}

