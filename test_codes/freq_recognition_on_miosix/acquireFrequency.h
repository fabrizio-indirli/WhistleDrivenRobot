#include <stdbool.h>
#include <cstdio>
#include "miosix.h"
#include <string>
#include <math.h>
#include "miosix/arch/common/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include "Microphone.h"
#include <stdio.h>
#include <iostream>
#define ARM_MATH_CM4
#include "miosix/arch/common/CMSIS/Include/arm_math.h"

#define SAMPLES 8192 //audio samples acquired each time
#define FFT_SIZE SAMPLES/2 //we store only the real parts of the samples


void startAcquisition(std::function<void ()> callbackFunction);

void stopAcquisition();
