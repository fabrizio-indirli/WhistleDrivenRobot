#include <Arduino.h>
#include "StringHandler.h"

StringHandler::StringHandler() {
    this->bufferIndex = 0;
    this->isOver = false;
}

void StringHandler::addChar(char ch) {
    //Control if the buffer is already full
    if(this->bufferIndex >= this->STRING_LENGTH) {
        this->isOver = true;
        return;
    }

    //Control if the buffer isn't full and if an end character is received
	if(!this->isOver && ch != this-> END_CHAR){
    	this->buffer[this->bufferIndex] = ch;
    	(this->bufferIndex)++;	
    }
    else
    	this->isOver = true;
}

String* StringHandler::getString() {
    this->buffer[this->bufferIndex] = '\0';
    this->bufferIndex = 0;
    this->isOver = false;
    String* string = new String(buffer);
    string->trim(); 
    return string;
}

bool StringHandler::areYouOver() {
    return this->isOver;
}
