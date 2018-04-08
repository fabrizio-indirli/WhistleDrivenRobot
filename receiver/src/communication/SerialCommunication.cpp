#include "SerialCommunication.h"
#include "../stringhandler/StringHandler.h"
#define CAPITAL_A 'A'
#define CAPITAL_Z 'Z'
#define NUMBER_0 '0' 
#define NUMBER_9 '9'
#define LOWER_A 'a'
#define LOWER_Z 'z'
#define END_C '\n'

SerialCommunication::SerialCommunication(){

}

bool SerialCommunication::isTheLineAllRead() {
    return this->stringHandler.areYouOver();
}

void SerialCommunication::putInTheBuffer(char c) {
    if(isThisCharGood(c))
    	this->stringHandler.addChar(c);
}

String* SerialCommunication::getReadLine() {
    return this->stringHandler.getString();
}

bool SerialCommunication::isThisCharGood(char c){
	return 	c == END_C
			||	isInTheInterval(c, CAPITAL_A, CAPITAL_Z)
			||	isInTheInterval(c, LOWER_A, LOWER_Z)
			||	isInTheInterval(c, NUMBER_0, NUMBER_9);
}

bool SerialCommunication::isInTheInterval(char c, char beginC, char endC){
	return c >= beginC && c<= endC;
}
