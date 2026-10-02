/*
 * can.c
 *
 *  Created on: Jul 29, 2026
 *      Author: nguyz
 */

#include "can.h"
#include "main.h"
extern CAN_HandleTypeDef hcan;

void CAN_Init(void) {
	CAN_FilterTypeDef filter;

	filter.FilterBank = 0;

	filter.FilterMode = CAN_FILTERMODE_IDMASK;

	filter.FilterScale = CAN_FILTERSCALE_32BIT;

	filter.FilterIdHigh = 0x0000;
	filter.FilterIdLow = 0x0000;

	filter.FilterMaskIdHigh = 0x0000;
	filter.FilterMaskIdLow = 0x0000;

	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;

	filter.FilterActivation = ENABLE;

	filter.SlaveStartFilterBank = 14;

	HAL_CAN_ConfigFilter(&hcan, &filter);

	HAL_CAN_Start(&hcan);

	HAL_CAN_ActivateNotification(&hcan,
	CAN_IT_RX_FIFO0_MSG_PENDING);
}
bool CAN_Send(const CAN_Frame_t *frame) {
	CAN_TxHeaderTypeDef txHeader;

	uint32_t mailbox;

	txHeader.StdId = frame->id;
	txHeader.ExtId = 0;

	txHeader.IDE = CAN_ID_STD;

	txHeader.RTR = CAN_RTR_DATA;

	txHeader.DLC = frame->dlc;

	txHeader.TransmitGlobalTime = DISABLE;

	if (HAL_CAN_AddTxMessage(&hcan, &txHeader, (uint8_t*) frame->data, &mailbox)
			!= HAL_OK) {
		return false;
	}

	while (HAL_CAN_IsTxMessagePending(&hcan, mailbox))
		;

	return true;
}
bool CAN_Receive(CAN_Frame_t *frame) {
	CAN_RxHeaderTypeDef rxHeader;

	if (HAL_CAN_GetRxFifoFillLevel(&hcan,
	CAN_RX_FIFO0) == 0) {
		return false;
	}

	if (HAL_CAN_GetRxMessage(&hcan,
	CAN_RX_FIFO0, &rxHeader, frame->data) != HAL_OK) {
		return false;
	}

	frame->id = rxHeader.StdId;

	frame->dlc = rxHeader.DLC;

	return true;
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_Frame_t frame;

	if (CAN_Receive(&frame)) {
		// xử lý frame
	}
}
