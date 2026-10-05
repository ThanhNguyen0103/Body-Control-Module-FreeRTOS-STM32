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

				if (frame.dlc >= 2) {
					uint16_t speed;

					speed = ((uint16_t) frame.data[1] << 8) | frame.data[0];

					event.type = BCM_EVENT_VEHICLE_SPEED;
					event.value = speed;

					xQueueSend(bcmEventQueue, &event, 0);
				}

				break;

			default:
				break;
			}
		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}
}
