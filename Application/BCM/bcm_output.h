/*
 * bcm_output.h
 *
 *  Created on: Oct 5, 2026
 *      Author: nguyz
 */

#ifndef BCM_BCM_OUTPUT_H_
#define BCM_BCM_OUTPUT_H_

#include "stdint.h"
#include "Common/types.h"
#include <stdbool.h>

typedef struct {
	IgnitionState_t ignition;
	Driving_Mode_t mode;
	uint16_t speed;
	bool brakePressed;

} BCM_Output_t;
#endif /* BCM_BCM_OUTPUT_H_ */
