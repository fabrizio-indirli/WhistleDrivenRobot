#include "BTSerialCommunication.h"

BTSerialCommunication::BTSerialCommunication(int tx_pin, int rx_pin)
        : SerialCommunication(tx_pin, rx_pin) {

    *bt =  SoftwareSerial(rx_pin, tx_pin);

}

void BTSerialCommunication::updateBuffer() {

}

bool BTSerialCommunication::isTheBufferAllRead() {
    return true;
}

char* BTSerialCommunication::getReadBuffer() {
    return 0;
}