#include <cstdio>
#include "miosix.h"
#include <string>
#include "freq_recognition/freq_recognition.h"
#include <util/lcd44780.h>
#include <stdio.h>
#include <display/display.h>
#include <ReceiverState.cpp>

#define SAMPLES 8192 //audio samples acquired each time
#define FFT_SIZE SAMPLES/2 //we store only the real parts of the samples

//this is the minimum amplitude of the acquired sound in order to activate the engines
#define AMPLITUDE_THRESHOLD 5000000

//if the sound's frequency is between these 2 values, the car will move forward
#define FORWARD_MIN_FREQ 300
#define FORWARD_MAX_FREQ 1000

//if the sound's frequency is between these 2 values, the car will turn left
#define TURNLEFT_MIN_FREQ 1200
#define TURNLEFT_MAX_FREQ 1800

//if the sound's frequency is between these 2 values, the car will turn right
#define TURNRIGHT_MIN_FREQ 2000
#define TURNRIGHT_MAX_FREQ 2800

//if the sound's frequency is between these 2 values, the car will move backwards
#define BACKWARD_MIN_FREQ 3000
#define BACKWARD_MAX_FREQ 4000

using namespace std;
using namespace miosix;

/**
 * This is the LED used to signal a valid command reception.
 */
typedef Gpio<GPIOD_BASE,12>  greenLed;

/**
 * This variable contains the amplitude of the last audio sample acquired.
 */
static float32_t fundamentalFreqAmplitude = 0;

/**
 * This variable stores the frequency of the last audio sample acquired.
 */
static float32_t freq=0;

/**
 * This is the display used to show the frequency and the command to execute
 */
Display display;

/**
 * This is the object that receives all the commands for the robots
 */
ReceiverState state;

/**
 * This function is invoked everytime a new audo sample has been acquired and processed.
 * when this function is called, the variables "freq" and "fundamentalFreqAmplitude" already contains
 * the frequency and the amplitude of the last acquired sample, respectively.
 */
void callback(){

    if(!(fundamentalFreqAmplitude>AMPLITUDE_THRESHOLD)){
    	display.setFrequencyTooLow();
    	display.setCommand(NONE);
    	return;	
    }

    if(freq < FORWARD_MAX_FREQ && freq > FORWARD_MIN_FREQ) {
            //if the frequency of the detected sound is between the FORWARD_MIN_FREQ and FORWARD_MAX_FREQ values, move forward
            greenLed::high();
			display.setCommand(FORWARD);
           	state.setState(FORWARD);//sends the 'move forward' command over bluetooth to the receiver
    }
    else if(freq>TURNLEFT_MIN_FREQ && freq < TURNLEFT_MAX_FREQ) {
            //if the frequency of the detected sound is between the TURNLEFT frequency values, turn left
            greenLed::high();
			display.setCommand(LEFT);
            state.setState(LEFT);//sends the 'turn left' command over bluetooth to the receiver
    }
    else if(freq>TURNRIGHT_MIN_FREQ && freq < TURNRIGHT_MAX_FREQ) {
            //if the frequency of the detected sound is between the TURNRIGHT frequency values, turn right
            greenLed::high();
			display.setCommand(RIGHT);
            state.setState(RIGHT);//sends the 'turn right' command over bluetooth to the receiver
    }
    else if(freq>BACKWARD_MIN_FREQ && freq<BACKWARD_MAX_FREQ) {
            //if the frequency of the detected sound is between the BACKWARD_MIN_FREQ and BACKWARD_MAX_FREQ values, move backwards
            greenLed::high();
            display.setCommand(BACK);
            state.setState(BACK);//sends the 'move backwards' command over bluetooth to the receiver
    }
    else {//if the sound is not strong enough, or if the frequency is not in the accepted ranges, stop all the engines
            greenLed::low();
			display.setCommand(STOP);
			state.setState(STOP);
    }

    //print on USB serial (for debugging)
    //printf("\nFrequency: %.2f hz, Amplitude: %.2f", freq, fundamentalFreqAmplitude);
    
    display.setFrequency(freq);
}

int main(int argc, char *argv[]){
	//greenLed setup
	greenLed::mode(Mode::OUTPUT);
	greenLed::low();

    //start audio frequency recognition
    startAcquisition(&callback, &freq, &fundamentalFreqAmplitude);

    while(true); //this in necessary because startAcquisition() works in another thread and it is non-blocking
    //maybe can be replaced with Thread::wait() ?
}



