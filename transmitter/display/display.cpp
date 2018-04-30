#include "display.h"

Display::Display(){
	display = new Lcd44780(	rs::getPin(),
							e::getPin(),
							d4::getPin(),
							d5::getPin(),
							d6::getPin(),
							d7::getPin(),
							HEIGHT,
							LENGTH);
  	command = UNDEFINED;
  	isFrequencyTooLow = true;
  	frequency = 1234;
}

void Display::setFrequencyTooLow(){
	isFrequencyTooLow = true;
	updateFrequency();
}

void Display::setFrequency(float32_t frequency){
	if(this->frequency == frequency && !(this->isFrequencyTooLow))
		return;
	
	this->frequency = frequency;
	this->isFrequencyTooLow = false;
	updateFrequency();
}

void Display::setCommand(Command command){
	if(this->command == command)
		return;
		
	this->command = command;
	updateCommand();
}

void Display::updateFrequency(){
	int n;
	display->go(0,0);
	if(!isFrequencyTooLow)
		display->printf("Freq: %.1f Hz%n", frequency, &n);
    else
    	display->printf("Volume too low%n", &n);
    fillDisplaySpace(n);
}

void Display::updateCommand(){
	int n = 0;
	display->go(0,1);
	switch(command){
		case FORWARD:	display->printf("Move Forward :D%n", &n);
						break;
		case BACK:		display->printf("Move Back :P%n", &n);
						break;
		case LEFT:		display->printf("Move Left :S%n", &n);
						break;
		case RIGHT:		display->printf("Move Right :O%n", &n);
						break;
		case STOP:		display->printf("Move Stop 8(%n", &n);
						break;
		case NONE:		display->printf("Nothing to do D:%n", &n);
						break;
		case UNDEFINED: display->printf("Problem :(%n", &n);
						break;
	}
	fillDisplaySpace(n);
}

void Display::fillDisplaySpace(int n){
	for(int i=LENGTH-n; i>0; i--)
    	display->printf(" ");
}

