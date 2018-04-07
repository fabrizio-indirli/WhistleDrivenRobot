#include <Arduino.h>
#include "BTSerialCommunication.h"

BTSerialCommunication::BTSerialCommunication(int tx_pin, int rx_pin): SerialCommunication() {
    pinMode(tx_pin, OUTPUT);
    pinMode(rx_pin, INPUT);
    bt = &SoftwareSerial(rx_pin, tx_pin);
    bt->begin(this->BR);
}

void BTSerialCommunication::updateBuffer() {
    //This cycle doesn't take characters if the building string is completed
    while(this->bt->available() > 0 && !(this->isTheLineAllRead())){
    	char c = this->bt->read();
        this->putInTheBuffer(c);
    }
}

void BTSerialCommunication::print(String* string) {
    this->bt->print(string->c_str());
}
