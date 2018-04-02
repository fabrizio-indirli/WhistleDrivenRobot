#include <Arduino.h>
#include "USBSerialCommunication.h"

USBSerialCommunication::USBSerialCommunication() : SerialCommunication() {
    Serial.begin(this->BR);
}

void USBSerialCommunication::updateBuffer() {
    while(Serial.available() > 0)
        this->putInTheBuffer(Serial.read());
}

void USBSerialCommunication::print(String *string) {
    Serial.print(string->c_str());
}