/*
 * can_task.c
 *
 *  Created on: Oct 5, 2026
 *      Author: nguyz
 */

#include "can_task.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Drivers/CAN/can.h"

#define CAN_RX_TASK_STACK_SIZE    128
#define CAN_RX_TASK_PRIORITY      2

static void CANTask(void *argument);

static CAN_Status_t latestStatus = { 0 };
static bool statusReceived = false;

void CANTask_Init(void) {
	xTaskCreate(CANTask, "CANTask",
	CAN_RX_TASK_STACK_SIZE,
	NULL,
	CAN_RX_TASK_PRIORITY,
	NULL);
}

bool CANTask_GetStatus(CAN_Status_t *status) {
	if (status == NULL) {
		return false;
	}

	taskENTER_CRITICAL();
	*status = latestStatus;
	bool received = statusReceived;
	taskEXIT_CRITICAL();

	return received;
}

static void CANTask(void *argument) {
	CAN_Frame_t frame;

	(void) argument;

	for (;;) {
		while (CAN_Receive(&frame)) {
			if (frame.id != CAN_ID_BCM_STATUS) {
				continue;
			}

			if (frame.dlc < 6) {
				continue;
			}

			CAN_Status_t newStatus;

			newStatus.ignition = frame.data[0];
			newStatus.mode = frame.data[1];

			newStatus.speed = ((uint16_t) frame.data[3] << 8)
					| (uint16_t) frame.data[2];

			newStatus.brakePressed = (frame.data[4] != 0U);

			newStatus.cruiseEnabled = (frame.data[5] != 0U);

			if (newStatus.ignition > 1U || newStatus.mode > 1U
					|| frame.data[4] > 1U || frame.data[5] > 1U) {
				continue;
			}

			taskENTER_CRITICAL();
			latestStatus = newStatus;
			statusReceived = true;
			taskEXIT_CRITICAL();
		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}
}
