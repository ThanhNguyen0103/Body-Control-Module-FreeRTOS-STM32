/*
 * can_task.h
 *
 *  Created on: Oct 5, 2026
 *      Author: nguyz
 */

#ifndef CAN_CAN_TASK_H_
#define CAN_CAN_TASK_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t ignition;
	uint8_t mode;
	uint16_t speed;
} CAN_Status_t;

void CANTask_Init(void);
bool CANTask_GetStatus(CAN_Status_t *status);
#endif /* CAN_CAN_TASK_H_ */
