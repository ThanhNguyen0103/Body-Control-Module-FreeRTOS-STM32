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

#define CAN_ID_IGNITION      	0x100
#define CAN_ID_MODE          	0x101
#define CAN_ID_VEHICLE_SPEED    0x102
#define CAN_ID_BRAKE        	0x103
#define CAN_ID_CRUISE        	0x104
#define CAN_ID_TURNSIGNAL    	0x105

typedef struct {
	uint32_t id;
	uint8_t dlc;
	uint8_t data[CAN_MAX_DATA_LENGTH];

} CAN_Frame_t;

void CAN_Init(void);

bool CAN_Send(const CAN_Frame_t *frame);

bool CAN_Receive(CAN_Frame_t *frame);

#endif /* DRIVERS_CAN_CAN_H_ */
