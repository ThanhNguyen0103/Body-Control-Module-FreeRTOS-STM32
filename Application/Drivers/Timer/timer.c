/*
 * timer.c
 *
 *  Created on: Aug 9, 2026
 *      Author: nguyz
 */

#include "timer.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;
static volatile bool speedUpdate = false;

void Timer_Init(void) {
	HAL_TIM_Base_Start_IT(&htim2);
}

bool Timer_IsSpeedUpdate(void) {
	return speedUpdate;
}

void Timer_ClearSpeedUpdate(void) {
	speedUpdate = false;
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//	if (htim->Instance == TIM2) {
//		speedUpdate = true;
//	}
//}
