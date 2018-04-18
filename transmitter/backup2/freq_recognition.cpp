#include "freq_recognition.h"

using namespace std;
using namespace miosix;

static std::tr1::function<void ()> callbackFun;
static float32_t* freq;
static float32_t* amplitude;
static float32_t input[SAMPLES]; //vector with PDM samples from the mic
static float32_t output[FFT_SIZE]; //vector with the amplitudes at each frequency
static float32_t sum[FFT_SIZE/8]; //used in the Harmonic Product Spectrum calculation

void startAcquisition(std::tr1::function<void ()> cback, float32_t* freqVar, float32_t* amplitudeVar)
{   Microphone& mic=Microphone::instance();


    callbackFun=cback;

    //mic is configured so that each time a new audio sample is acquired:
    //1) its frequency and amplitude are saved in the specified variables
    // and 2) the callback function is executed
    mic.init(&FFTandCallback, FFT_SIZE);


    //start acquisition
    mic.start();
}


void maxFreq(float32_t *PCMsample, uint32_t size)
{
    uint32_t f;
    PCMsample[0]=0;
    arm_max_f32(output, size/LP_DIV, amplitude, &f);
    *freq=f * SAMPLING_FREQ / size;
}

void FFTandCallback(uint16_t* PCMvector, uint16_t  size)
{
       arm_cfft_radix4_instance_f32 S;    // ARM CFFT module
    uint16_t i;

    // first step is to adjust the pcm vector to the format of the fft function, alternating real and imaginary parts
    for(i = 0; i < FFT_SIZE; i++)
    {
        /* Real part, make offset by ADC / 2 */
        input[(uint16_t)i*2] = (float32_t)((float32_t) PCMvector[i] - 32768);
        /* imaginary part */
        input[(uint16_t)((i*2)+1)] = 0;
    }
    //Calculate the frequency using FFT
    // Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1
    arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
    // Process the data through the CFFT/CIFFT module
    arm_cfft_radix4_f32(&S, input);
    // Process the data through the Complex Magnitude Module for calculating the magnitude at each bin
    arm_cmplx_mag_f32(input, output, FFT_SIZE);

    // Calculates fundamental frequency and amplitude of the sample, and stores the value in *freq and *amplitude, respectively
    maxFreq(output, FFT_SIZE);

    callbackFun();
}
