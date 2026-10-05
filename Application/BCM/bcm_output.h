/*
 * bcm_output.h
 *
 *  Created on: Oct 5, 2026
 *      Author: nguyz
 */

#ifndef BCM_BCM_OUTPUT_H_
#define BCM_BCM_OUTPUT_H_

typedef enum {
	BCM_OUTPUT_IGNITION_OFF = 0,
	BCM_OUTPUT_IGNITION_ON,
	BCM_OUTPUT_DRIVING_MODE_ECO,
	BCM_OUTPUT_DRIVING_MODE_SPORT

} BCM_OutputType_t;

typedef struct {
	BCM_OutputType_t type;

} BCM_Output_t;

#endif /* BCM_BCM_OUTPUT_H_ */
