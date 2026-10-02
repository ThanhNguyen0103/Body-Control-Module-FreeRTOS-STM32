/*
 * can.h
 *
 *  Created on: Jul 29, 2026
 *      Author: nguyz
 */

#ifndef DRIVERS_CAN_CAN_H_
#define DRIVERS_CAN_CAN_H_

#include "stdbool.h"
#include "stdint.h"
#define CAN_MAX_DATA_LENGTH 8

typedef struct {
	uint32_t id;
	uint8_t dlc;
	uint8_t data[CAN_MAX_DATA_LENGTH];

} CAN_Frame_t;

void CAN_Init(void);

bool CAN_Send(const CAN_Frame_t *frame);

bool CAN_Receive(CAN_Frame_t *frame);

#endif /* DRIVERS_CAN_CAN_H_ */
