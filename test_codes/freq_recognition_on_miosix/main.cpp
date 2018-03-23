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

#define SAMPLES 8192
#define FFT_SIZE SAMPLES/8

/*Collegare cavo USB TTL ai pin PA2 e PA3 della scheda e usare un monitor seriale con baud rate 19200*/

using namespace std;
using namespace miosix;



static short empty = 0;
static float currentNote;
static float32_t input[SAMPLES];
static float32_t output[FFT_SIZE];
static float32_t sum[FFT_SIZE/8];
typedef Gpio<GPIOD_BASE,12>  greenLed;

void frequencyRead(uint16_t*, uint16_t);

int main()
{	
	greenLed::mode(Mode::OUTPUT);
    Microphone& mic = Microphone::instance();
    mic.init(&frequencyRead, FFT_SIZE);
	
    mic.start();
	while(true);
	mic.stop();
	


}

//Harmonic Product Spectrum
/* to find the fundamental frequency that isn't always the strongest one */
float32_t hps(float32_t *magFFT, int size)
{
    float32_t freq, maxValue =0;

    for(int k = 0; k < size/8; k++)
    {
	sum[k] = magFFT[k] * magFFT[2*k] * magFFT[3*k];
	// find fundamental frequency
	if( sum[k] > maxValue && k > 0 )
	{
  	    maxValue = sum[k];
	    freq = k;
	}
    }
    return freq * 11000 / size;
}

void frequencyRead(uint16_t* PCM, uint16_t  size)
{	
    arm_cfft_radix4_instance_f32 S;    // ARM CFFT module
    uint16_t i;
    float32_t freq;

    // first step is to adjust the pcm vector to the format of the fft function, alternating rea and imaginary parts
    for(i = 0; i < size; i++)
    {
        /* Real part, make offset by ADC / 2 */
        input[(uint16_t)i*2] = (float32_t)((float32_t) PCM[i] - 32768);
        /* imaginary part */
        input[(uint16_t)((i*2)+1)] = 0;
    }
    // now we calculate the frequency using FFT, the code below is pretty much a standard one and can be find in various websites
    // Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1
    arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
    // Process the data through the CFFT/CIFFT module
    arm_cfft_radix4_f32(&S, input);
    // Process the data through the Complex Magniture Module for calculating the magnitude at each bin
    arm_cmplx_mag_f32(input, output, FFT_SIZE);
    // Calculates maxValue and returns corresponding value
    //arm_max_f32(output, FFT_SIZE, &maxValue, &maxIndex);
    freq = hps(output, FFT_SIZE);
    printf("\nFrequency: %.2f hz", freq);
	
}
