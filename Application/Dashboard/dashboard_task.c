/*
 * dashboard_task.c
 *
 *  Created on: Oct 9, 2026
 *      Author: nguyz
 */

#include "dashboard_task.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "CAN/can_task.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#define DASHBOARD_TASK_STACK_SIZE    256
#define DASHBOARD_TASK_PRIORITY      1

/* Refresh interval */
#define DASHBOARD_REFRESH_MS         100

static void DashboardTask(void *argument);

static void Dashboard_ShowWaiting(void);
static void Dashboard_ShowStatus(const CAN_Status_t *status);

void DashboardTask_Init(void) {
	xTaskCreate(DashboardTask, "DashboardTask",
	DASHBOARD_TASK_STACK_SIZE,
	NULL,
	DASHBOARD_TASK_PRIORITY,
	NULL);
}

static void DashboardTask(void *argument) {
	CAN_Status_t status;

	bool waitingScreenShown = false;
	bool statusScreenShown = false;

	CAN_Status_t previousStatus = { 0 };

	(void) argument;

	/* Initialize OLED */
	ssd1306_Init();

	Dashboard_ShowWaiting();

	for (;;) {
		if (CANTask_GetStatus(&status)) {
			bool changed = false;

			if (!statusScreenShown) {
				changed = true;
			} else if ((status.ignition != previousStatus.ignition)
					|| (status.mode != previousStatus.mode)
					|| (status.speed != previousStatus.speed)
					|| (status.brakePressed != previousStatus.brakePressed)
					|| (status.cruiseEnabled != previousStatus.cruiseEnabled)) {
				changed = true;
			}

			if (changed) {

				Dashboard_ShowStatus(&status);

				previousStatus = status;
				statusScreenShown = true;
			}

			waitingScreenShown = false;
		} else if (!waitingScreenShown) {

			Dashboard_ShowWaiting();

			waitingScreenShown = true;
			statusScreenShown = false;
		}

		vTaskDelay(pdMS_TO_TICKS(DASHBOARD_REFRESH_MS));
	}
}

static void Dashboard_ShowWaiting(void) {
	ssd1306_Fill(Black);

	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("BCM DASHBOARD", Font_7x10, White);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("WAITING CAN...", Font_7x10, White);

	ssd1306_UpdateScreen();
}

static void Dashboard_ShowStatus(const CAN_Status_t *status) {
	char buffer[24];

	const char *ignitionText;
	const char *modeText;
	const char *brakeText;
	const char *cruiseText;

	if (status == NULL) {
		return;
	}

	ignitionText = (status->ignition == 1U) ? "ON" : "OFF";
	modeText = (status->mode == 1U) ? "SPORT" : "ECO";
	brakeText = status->brakePressed ? "ON" : "OFF";
	cruiseText = status->cruiseEnabled ? "ON" : "OFF";
	ssd1306_Fill(Black);

	/* Title */
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("BCM DASHBOARD", Font_7x10, White);

	/* Ignition */
	ssd1306_SetCursor(0, 14);
	(void) snprintf(buffer, sizeof(buffer), "IGN: %s", ignitionText);
	ssd1306_WriteString(buffer, Font_7x10, White);

	/* Driving mode */
	ssd1306_SetCursor(0, 26);
	(void) snprintf(buffer, sizeof(buffer), "MODE: %s", modeText);
	ssd1306_WriteString(buffer, Font_7x10, White);

	/* Vehicle speed */
	ssd1306_SetCursor(0, 38);
	(void) snprintf(buffer, sizeof(buffer), "SPEED: %u km/h",
			(unsigned int) status->speed);
	ssd1306_WriteString(buffer, Font_7x10, White);

	/* Brake */
	ssd1306_SetCursor(0, 50);
	(void) snprintf(buffer, sizeof(buffer), "BRAKE: %s", brakeText);
	ssd1306_WriteString(buffer, Font_7x10, White);

	ssd1306_SetCursor(0, 58);
	(void) snprintf(buffer, sizeof(buffer), "CRUISE: %s", cruiseText);
	ssd1306_WriteString(buffer, Font_7x10, White);
	ssd1306_UpdateScreen();
}
