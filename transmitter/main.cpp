#include <cstdio>
#include "miosix.h"
#include <string>
#include "Microphone.h"
#include <stdio.h>

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
typedef Gpio<GPIOD_BASE,12>  greenLed; //used to notify that the mic has acquired a sound whose amplitude is > AMPLITUDE_THRESHOLD
typedef Gpio<GPIOD_BASE,2>  leftEngineForward; //PD2 pin commands forward movement of the left engine
typedef Gpio<GPIOD_BASE,6>  leftEngineBackward; //PD6 pin commands backward movement of the left engine
typedef Gpio<GPIOD_BASE,3>  rightEngineForward; //PD3 pin commands forward movement of the right engine
typedef Gpio<GPIOD_BASE,4>  rightEngineBackward; //PD4 pin commands backward movement of the right engine
static float32_t fundamentalFreqAmplitude=0; //stores the amplitude of the last audio sample acquired
static float32_t freq=0; //stores frequency of the last audio sample acquired

void callback()
{//function that is called each time a new audio sample has been acquired and processed
//when this function is called, the variables "freq" and "fundamentalFreqAmplitude" already contain
//the frequency and the amplitude of the last acquired sample, respectively

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

    //mic initialization: create microphone object and set it
    Microphone& mic = Microphone::instance();

    //mic is configured so that each time a new audio sample is acquired:
    //1) its frequency and amplitude are saved in the specified variables
    // and 2) the callback function is executed
    mic.init(&callback, &freq, &fundamentalFreqAmplitude);


    //start acquisition
    mic.start();
	while(true);
	mic.stop();



}



