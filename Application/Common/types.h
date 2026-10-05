/*
 * types.h
 *
 *  Created on: Aug 7, 2026
 *      Author: nguyz
 */

#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#include "stdbool.h"
#include "stdint.h"

typedef enum {
	IGNITION_OFF, IGNITION_ON
} IgnitionState_t;

typedef enum {
	TURN_OFF, TURN_LEFT, TURN_RIGHT, TURN_HAZARD
} TurnSignalState_t;

typedef enum {
	CRUISE_OFF, CRUISE_ON
} Cruise_State_t;

typedef enum {
	ECO_MODE, SPORT_MODE,

} Driving_Mode_t;

typedef enum {
	BRAKE_OFF, BRAKE_ON
} Brake_State_t;

typedef struct {
	IgnitionState_t ignition;
	Driving_Mode_t mode;
	uint8_t speed;
	bool cruise;
	TurnSignalState_t signal;
} DashboardData_t;

#endif /* COMMON_TYPES_H_ */
