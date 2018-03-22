#ifndef _INITIALIZATION_H_
#define _INITIALIZATION_H_
#include "pdm_filter.h"

static void RCC_Configure(void);

static void NVIC_Configure(void);

static void GPIO_Configure(void);

static void I2S_Configure(void);

static void PDM_Lib_Init(uint16_t sampling_freq, float HPfilter_cut_freq, float LPfilter_cutoff_freq, PDMFilter_InitStruct* Filter);

#endif // _INITIALIZATION_H_



