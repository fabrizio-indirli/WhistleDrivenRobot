#include <Arduino.h>
#include <SoftwareSerial.h>
#include "optimized-communication.h"

OptimizedCommunication::OptimizedCommunication(){
	pinMode(BT_RX_PIN, INPUT);
	pinMode(BT_TX_PIN, OUTPUT);
	this->bt.begin(this->BR);
}
	
bool OptimizedCommunication::doYouHaveCharacters(){
	return this->bt.available();
}
	
char OptimizedCommunication::read(){
	return this->bt.read();
}

bool isALowerCaseLitteralChar(char c){
	return c >='a' && c <= 'z';
}
