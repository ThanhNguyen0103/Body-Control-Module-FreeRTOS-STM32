/*
 * timer.h
 *
 *  Created on: Aug 9, 2026
 *      Author: nguyz
 */

#ifndef DRIVERS_TIMER_TIMER_H_
#define DRIVERS_TIMER_TIMER_H_

#include <stdbool.h>

void Timer_Init(void);

bool Timer_IsSpeedUpdate(void);

void Timer_ClearSpeedUpdate(void);

#endif /* DRIVERS_TIMER_TIMER_H_ */
