/**This library defines a simple interface for recording audio with the embedded
* microphone on the STM32F4 Discovery board and form performing an FFT analysis to
* calculate the strongest frequency (and its amplitude) of each sample **/


#include "../miosix/miosix.h"
#include "Microphone.h"
#include <tr1/functional>
#include <cstdio>
#include <stdbool.h>
#include <math.h>
#define ARM_MATH_CM4
#include "../miosix/arch/common/CMSIS/Include/arm_math.h"

#define SAMPLES 8192 //audio samples acquired each time
#define FFT_SIZE SAMPLES/2 //we store only the real parts of the samples

#define DECIMATION_FACTOR       64
#define SAMPLING_FREQ            44000

/*this is the low-pass divider: it defines which portion of the output[] vector will be taken into account
* when computing the strongest frequency of each sample.
* For example, if LP_DIV=1, all the frequencies (the entire output[] vector) will be taken into account;
* if LP_DIV = 2, only the first half of the frequencies will be taken into account
* if LP_DIV = 4, only the first quarter of the output[] vector will be taken into account
*/
#define LP_DIV 4


void startAcquisition(function<void ()> cback, float32_t* freqVar, float32_t* amplitudeVar);

void stopAcquisition();

void FFTandCallback(uint16_t* PCMvector, uint16_t  size);
