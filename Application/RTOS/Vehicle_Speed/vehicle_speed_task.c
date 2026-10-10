/*
 * vehicle_speed_task.c
 *
 *  Created on: Oct 5, 2026
 *      Author: nguyz
 */

#include "vehicle_speed_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "BCM/bcm.h"
#include "BCM/bcm_event.h"

#include "Drivers/ADC/adc.h"

static void VehicleSpeedTask(void *argument);

void VehicleSpeedTask_Init(void) {
	ADC_Init();
	xTaskCreate(VehicleSpeedTask, "VehicleSpeed", 128,
	NULL, 2,
	NULL);
}
static void VehicleSpeedTask(void *argument) {
	uint32_t adcValue;
	BCM_Event_t event;

	for (;;) {
		adcValue = ADC_ReadValue();
		event.type = BCM_EVENT_SPEED_INPUT;
		event.value = (uint16_t) adcValue;

		xQueueSend(bcmEventQueue, &event, 0);

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
