#include <cstdio>
#include "miosix.h"
#include <string>
#include "freq_recognition.h"
#include <util/lcd44780.h>
#include <stdio.h>

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


/*Collegare cavo USB TTL ai pin PA2 e PA3 della scheda e usare un monitor seriale con baud rate 19200*/

using namespace std;
using namespace miosix;


char commandString[30];

typedef Gpio<GPIOD_BASE,12>  greenLed; //on when a valid command is generated

//pins to drive the hd44780 display
typedef Gpio<GPIOE_BASE,11> d4; //connect display's d4 pin (pin 11) to PE11 port of the board
typedef Gpio<GPIOE_BASE,12> d5; //connect display's d5 pin (pin 12) to PE12 port of the board
typedef Gpio<GPIOE_BASE,13> d6; //connect display's d6 pin (pin 13) to PE13 port of the board
typedef Gpio<GPIOE_BASE,14> d7; //connect display's d7 pin (pin 14) to PE14 port of the board
typedef Gpio<GPIOE_BASE,7> rs; //connect display's rs pin (pin 4) to PE7 port of the board
typedef Gpio<GPIOE_BASE,8> e; //connect display's E pin (pin 6) to PE8 port of the board
//other connections for the display: connect VSS (pin 1) to GND
//connect VDD (pin 2) of the display to 5V
//connect display's V0 (pin 3) to the center tap of a potentiometer with the other two ends connected to 5V and GND
//connect display's RW (pin 5) to GND
//if present, connect display's pin A (pin 15) to 3.3V and pin K (PIN 16) to GND

Lcd44780* display; //object that commands the HD44780 display

static float32_t fundamentalFreqAmplitude=0; //stores the amplitude of the last audio sample acquired
static float32_t freq=0; //stores frequency of the last audio sample acquired
static int overThreshold = 0; //each time a new sample is analyzed,this will be set to 1 if the sample's amplitude > AMPLITUDE_THRESHOLD

void callback()
{//function that is called each time a new audio sample has been acquired and processed
//when this function is called, the variables "freq" and "fundamentalFreqAmplitude" already contain
//the frequency and the amplitude of the last acquired sample, respectively

    if(fundamentalFreqAmplitude>AMPLITUDE_THRESHOLD) overThreshold=1;
    else overThreshold=0;

    if(overThreshold && freq < FORWARD_MAX_FREQ && freq > FORWARD_MIN_FREQ) {
            //if the frequency of the detected sound is between the FORWARD_MIN_FREQ and FORWARD_MAX_FREQ values, move forward
            greenLed::high();

           printf("f\n");//sends the 'move forward' command over bluetooth to the receiver


            strcpy(commandString, "move forward");
    }
    else if(overThreshold && freq>TURNLEFT_MIN_FREQ && freq < TURNLEFT_MAX_FREQ) {
            //if the frequency of the detected sound is between the TURNLEFT frequency values, turn left
            greenLed::high();

            printf("l\n");//sends the 'turn left' command over bluetooth to the receiver


            strcpy(commandString, "turn left");
    }
    else if(overThreshold && freq>TURNRIGHT_MIN_FREQ && freq < TURNRIGHT_MAX_FREQ) {
            //if the frequency of the detected sound is between the TURNRIGHT frequency values, turn right
            greenLed::high();

            printf("r\n");//sends the 'turn right' command over bluetooth to the receiver


            strcpy(commandString, "turn right");
    }
    else if(overThreshold && freq>BACKWARD_MIN_FREQ && freq<BACKWARD_MAX_FREQ) {
            //if the frequency of the detected sound is between the BACKWARD_MIN_FREQ and BACKWARD_MAX_FREQ values, move backwards
            greenLed::high();

            printf("b\n");//sends the 'move backwards' command over bluetooth to the receiver


            strcpy(commandString, "move backwards");
    }
    else {//if the sound is not strong enough, or if the frequency is not in the accepted ranges, stop all the engines
            greenLed::low();
			
			printf("s\n");
			
            strcpy(commandString, "No command");
    }

    //print on USB serial (for debugging)
    //printf("\nFrequency: %.2f hz, Amplitude: %.2f, Command: %s", freq, fundamentalFreqAmplitude, commandString);

    //print on display
    display->clear();
    display->go(0,0);
    if(overThreshold) display->printf("Freq: %.1f Hz", freq);
    else display->printf("Volume too low", freq);
    display->go(0,1);
    display->printf("%s", commandString);
}

int main()
{   //greenLed setup
	greenLed::mode(Mode::OUTPUT);
	greenLed::low();

    //initialize display
    display = new Lcd44780(rs::getPin(), e::getPin(), d4::getPin(), d5::getPin(), d6::getPin(), d7::getPin(), 2, 16);
    display->clear();
    display->go(0,0);

    //start audio frequency recognition
    startAcquisition(&callback, &freq, &fundamentalFreqAmplitude);

    while(true); //this in necessary because startAcquisition() works in another thread and it is non-blocking
    //maybe can be replaced with Thread::wait() ?
}



