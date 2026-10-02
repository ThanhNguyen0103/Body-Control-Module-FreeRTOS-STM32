/*
 * adc.h
 *
 *  Created on: Jul 12, 2026
 *      Author: nguyz
 */

#ifndef DRIVERS_ADC_ADC_H_
#define DRIVERS_ADC_ADC_H_
#include "main.h"
#include "stdint.h"

uint32_t ADC_ReadValue(void);
void ADC_Init(void);
#endif /* DRIVERS_ADC_ADC_H_ */
