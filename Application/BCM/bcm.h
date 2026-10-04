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

extern QueueHandle_t bcmEventQueue;

void BCM_Init(void);

#endif /* BCM_BCM_H_ */
