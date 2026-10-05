/*
 * bcm.h
 *
 *  Created on: Oct 4, 2026
 *      Author: nguyz
 */

#ifndef BCM_BCM_H_
#define BCM_BCM_H_

#include "FreeRTOS.h"
#include "queue.h"

#include "bcm_event.h"
#include "bcm_output.h"

extern QueueHandle_t bcmEventQueue;
extern QueueHandle_t bcmOutputQueue;

void BCM_Init(void);

#endif /* BCM_BCM_H_ */
