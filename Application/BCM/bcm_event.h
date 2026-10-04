/*
 * bcm_event.h
 *
 *  Created on: Oct 4, 2026
 *      Author: nguyz
 */

#ifndef BCM_BCM_EVENT_H_
#define BCM_BCM_EVENT_H_

typedef enum {
	BCM_EVENT_NONE = 0,
	BCM_EVENT_IGNITION_PRESSED,
	BCM_EVENT_DRIVING_MODE_PRESSED

} BCM_EventType_t;

typedef struct {
	BCM_EventType_t type;

} BCM_Event_t;

#endif /* BCM_BCM_EVENT_H_ */
