#include <Arduino.h>
#include "StringHandler.h"

StringHandler::StringHandler() {
    this->bufferIndex = 0;
    this->isOver = false;
}

void StringHandler::addChar(char ch) {
    if(this->bufferIndex >= this->STRING_LENGTH) {
        this->isOver = true;
        return;
    }

	if(!this->isOver && ch != this-> END_CHAR)
    	this->buffer[this->bufferIndex] = ch;
    else{
    	this->isOver = true;
    	Serial.print("I'm over: ");
    }
    (this->bufferIndex)++;
}

String* StringHandler::getString() {
    this->buffer[this->bufferIndex] = '\0';
    this->bufferIndex = 0;
    this->isOver = false;
    return new String(buffer);
}

bool StringHandler::areYouOver() {
    return this->isOver;
}
