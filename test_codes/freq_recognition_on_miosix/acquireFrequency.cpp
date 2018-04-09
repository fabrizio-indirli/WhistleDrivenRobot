#include "acquireFrequency.h"

static float32_t input[SAMPLES]; //vector with PDM samples from the mic
static float32_t output[FFT_SIZE]; //vector with the amplitudes at each frequency
static float32_t sum[FFT_SIZE/8]; //used in the Harmonic Product Spectrum calculation

using namespace std;
using namespace miosix;

Microphone& mic;

//Harmonic Product Spectrum
/* to find the fundamental frequency that isn't always the strongest one */
float32_t hps(float32_t *magFFT, int size, float32_t* amplitudeVar)
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
    *amplitudeVar=magFFT[(int)freq];
    return freq * 11000 / size;
}


void frequencyRead(uint16_t* PCM, uint16_t  size, std::function<void ()> callbackFunction, float32_t* freq, float32_t* amplitudeVar)
{
    arm_cfft_radix4_instance_f32 S;    // ARM CFFT module
    uint16_t i;
    *freq =0;

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
    *freq = hps(output, FFT_SIZE, amplitudeVar);
    callBackFunction();
}

void startAcquisition(std::function<void ()> callbackFunction)
{
    //mic initialization
    mic = Microphone::instance();
    mic.init(&frequencyRead, FFT_SIZE, &callbackFunction);

    //start acquisition
    mic.start();
}

void stopAcquisition()
{
    mic.stop();
}
