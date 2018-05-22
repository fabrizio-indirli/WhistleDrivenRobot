#include <Arduino.h>
#include <SoftwareSerial.h>
#include "optimized-communication.h"
#define LOWER_A 'a'
#define LOWER_Z 'z'

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

bool isALowerCaseLiteralChar(char c){
	return c >=LOWER_A && c <= LOWER_Z;
}
