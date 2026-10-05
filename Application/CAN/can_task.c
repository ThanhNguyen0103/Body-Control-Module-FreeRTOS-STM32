/*
 * can_task.c
 *
 *  Created on: Oct 5, 2026
 *      Author: nguyz
 */

#include "main.h"
#include "Drivers/CAN/can.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "BCM/bcm_event.h"
#include "BCM/bcm.h"

#include "can_task.h"

static void CANTask(void *argument);

void CANTask_Init(void) {

	xTaskCreate(CANTask, "CANTask", 128,
	NULL, 2,
	NULL);
}
static void CANTask(void *argument) {
	CAN_Frame_t frame;
	BCM_Event_t event;

	for (;;) {
		if (CAN_Receive(&frame)) {
			switch (frame.id) {
			case CAN_ID_VEHICLE_SPEED:

				break;

			default:
				break;
			}
		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}
}
