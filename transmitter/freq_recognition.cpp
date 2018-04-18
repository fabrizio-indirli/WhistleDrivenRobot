#include "freq_recognition.h"

using namespace std;
using namespace miosix;

static std::tr1::function<void ()> callbackFun;
static float32_t* freq;
static float32_t* amplitude;
static float32_t input[SAMPLES]; //vector with PDM samples from the mic
static float32_t output[FFT_SIZE]; //vector with the amplitudes at each frequency

/**Starts the acquisition and the analysis of the audio samples.
*This is done in another thread: this function is non-blocking! **/
void startAcquisition(std::tr1::function<void ()> cback, float32_t* freqVar, float32_t* amplitudeVar)
{   //get instance of the Microphone class, which is a singleton
    Microphone& mic=Microphone::instance();

    //stores parameters in global variables
    freq=freqVar;
    amplitude=amplitudeVar;
    callbackFun=cback;

    //mic is configured so that each time a new audio sample is acquired the callback function is executed
    mic.init(&FFTandCallback, FFT_SIZE);

    //start acquisition
    mic.start();
}

/**This function calculates the strongest frequency of the sample and stores its value in the 'freq' variable.
* It also stores the amplitude of that frequency in the 'amplitude' variable **/
void maxFreq(float32_t *PCMsample, uint32_t size)
{
    uint32_t f;
    PCMsample[0]=0; //in position 0 there is always 'infinite', we must ignore it
    arm_max_f32(output, size/LP_DIV, amplitude, &f); //amplitude=max value in the vector, f=its position in the vector
    *freq=f * SAMPLING_FREQ / size;
}

void FFTandCallback(uint16_t* PCMvector, uint16_t  size)
{
       arm_cfft_radix4_instance_f32 S;    // ARM CFFT module
    uint16_t i;

    // first step is to adjust the pcm vector to the format of the fft function, alternating real and imaginary parts
    for(i = 0; i < size; i++)
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

void stopAcquisition()
{   //get instance of the Microphone class, which is a singleton
    Microphone& mic=Microphone::instance();

    //stop the acquisition
    mic.stop();

}
