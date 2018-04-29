#include <Arduino.h>
#include "BTSerialCommunication.h"

BTSerialCommunication::BTSerialCommunication(): SerialCommunication() {
    pinMode(BT_RX_PIN, INPUT);
  	pinMode(BT_TX_PIN, OUTPUT);
    this->bt.begin(this->BR);
}

void BTSerialCommunication::updateBuffer() {
    //This cycle doesn't take characters if the building string is completed
    while(this->bt.available() > 0 && !(this->isTheLineAllRead())){
    	char c = this->bt.read();
        this->putInTheBuffer(c);
    }
}

void BTSerialCommunication::print(String* string) {
    this->bt.print(string->c_str());
}
