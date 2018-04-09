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

//this is the minimum amplitude of the acquired sound in order to activate the engines
#define AMPLITUDE_THRESHOLD 10000000

#define FORWARD_MAX_FREQ 300 //if the sound's frequency is below this value, the car will move forward

//if the sound's frequency is between these 2 values, the car will turn left
#define TURNLEFT_MIN_FREQ 350
#define TURNLEFT_MAX_FREQ 400

//if the sound's frequency is between these 2 values, the car will turn right
#define TURNRIGHT_MIN_FREQ 450
#define TURNRIGHT_MAX_FREQ 500

//if the sound's frequency is over this value, the car will move backwards
#define BACKWARD_MIN_FREQ 550


/*Collegare cavo USB TTL ai pin PA2 e PA3 della scheda e usare un monitor seriale con baud rate 19200*/

using namespace std;
using namespace miosix;


char commandString[30];
static float32_t input[SAMPLES]; //vector with PDM samples from the mic
static float32_t output[FFT_SIZE]; //vector with the amplitudes at each frequency
static float32_t sum[FFT_SIZE/8]; //used in the Harmonic Product Spectrum calculation
typedef Gpio<GPIOD_BASE,12>  greenLed; //used to notify that the mic has acquired a sound whose amplitude is > AMPLITUDE_THRESHOLD
typedef Gpio<GPIOD_BASE,2>  leftEngineForward; //PD2 pin commands forward movement of the left engine
typedef Gpio<GPIOD_BASE,6>  leftEngineBackward; //PD6 pin commands backward movement of the left engine
typedef Gpio<GPIOD_BASE,3>  rightEngineForward; //PD3 pin commands forward movement of the right engine
typedef Gpio<GPIOD_BASE,4>  rightEngineBackward; //PD4 pin commands backward movement of the right engine
static float32_t fundamentalFreqAmplitude=0;

void frequencyRead(uint16_t*, uint16_t);

int main()
{   //greenLed and engine pins setup
	greenLed::mode(Mode::OUTPUT);
	greenLed::low();
	leftEngineForward::mode(Mode::OUTPUT);
	leftEngineForward::low();
	leftEngineBackward::mode(Mode::OUTPUT);
	leftEngineBackward::low();
	rightEngineForward::mode(Mode::OUTPUT);
	rightEngineForward::low();
	rightEngineBackward::mode(Mode::OUTPUT);
	rightEngineBackward::low();

    //mic initialization
    Microphone& mic = Microphone::instance();
    mic.init(&frequencyRead, FFT_SIZE);

    //start acquisition
    mic.start();
	while(true);
	mic.stop();



}

//Harmonic Product Spectrum
/* to find the fundamental frequency that isn't always the strongest one */
float32_t hps(float32_t *magFFT, int size)
{
    float32_t freq, maxValue =0;
    freq=0;
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
    fundamentalFreqAmplitude=magFFT[(int)freq];
    return freq * 11000 / size;
}


void frequencyRead(uint16_t* PCM, uint16_t  size)
{
    arm_cfft_radix4_instance_f32 S;    // ARM CFFT module
    uint16_t i;
    float32_t freq; //variable where to store the fundamental frequency of the current sound sample

    // first step is to adjust the pcm vector to the format of the fft function, alternating real and imaginary parts
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
    // Process the data through the Complex Magnitude Module for calculating the magnitude at each bin
    arm_cmplx_mag_f32(input, output, FFT_SIZE);
    // Calculates maxValue and returns corresponding value
    freq = hps(output, FFT_SIZE);

    if(fundamentalFreqAmplitude>AMPLITUDE_THRESHOLD && freq < FORWARD_MAX_FREQ) {
            //if the frequency of the detected sound is less than FORWARD_MAX_FREQ, move forward
            greenLed::high();
            leftEngineForward::high();
            leftEngineBackward::low();
            rightEngineForward::high();
            rightEngineBackward::low();
            strcpy(commandString, "move forward");
    }
    else if(fundamentalFreqAmplitude>AMPLITUDE_THRESHOLD && freq>TURNLEFT_MIN_FREQ && freq < TURNLEFT_MAX_FREQ) {
            //if the frequency of the detected sound is between the TURNLEFT frequency values, turn left
            greenLed::high();
            leftEngineForward::low();
            leftEngineBackward::low();
            rightEngineForward::high();
            rightEngineBackward::low();
            strcpy(commandString, "turn left");
    }
    else if(fundamentalFreqAmplitude>AMPLITUDE_THRESHOLD && freq>TURNRIGHT_MIN_FREQ && freq < TURNRIGHT_MAX_FREQ) {
            //if the frequency of the detected sound is between the TURNRIGHT frequency values, turn right
            greenLed::high();
            leftEngineForward::high();
            leftEngineBackward::low();
            rightEngineForward::low();
            rightEngineBackward::low();
            strcpy(commandString, "turn right");
    }
    else if(fundamentalFreqAmplitude>AMPLITUDE_THRESHOLD && freq>BACKWARD_MIN_FREQ) {
            //if the frequency of the detected sound is greater than BACKWARD_MIN_FREQ, move backwards
            greenLed::high();
            leftEngineForward::low();
            leftEngineBackward::high();
            rightEngineForward::low();
            rightEngineBackward::high();
            strcpy(commandString, "move backwards");
    }
    else {//if the sound is not strong enough, or if the frequency is not in the accepted ranges, stop all the engines
            greenLed::low();
            leftEngineForward::low();
            leftEngineBackward::low();
            rightEngineForward::low();
            rightEngineBackward::low();
            strcpy(commandString, "none");
    }
    printf("\nFrequency: %.2f hz, Amplitude: %.2f, Command: %s", freq, fundamentalFreqAmplitude, commandString);
}
