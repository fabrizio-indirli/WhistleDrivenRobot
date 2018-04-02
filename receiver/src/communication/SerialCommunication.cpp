#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(int tx_pin, int rx_pin){
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;
}