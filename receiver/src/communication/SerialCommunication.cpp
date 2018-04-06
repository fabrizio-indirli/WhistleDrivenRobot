#include <Arduino.h>
#include "SerialCommunication.h"
#include "../stringhandler/StringHandler.h"

SerialCommunication::SerialCommunication(){

}

bool SerialCommunication::isTheLineAllRead() {
    return this->stringHandler.areYouOver();
}

void SerialCommunication::putInTheBuffer(char c) {
    this->stringHandler.addChar(c);
}

String* SerialCommunication::getReadLine() {
    return this->stringHandler.getString();
}
