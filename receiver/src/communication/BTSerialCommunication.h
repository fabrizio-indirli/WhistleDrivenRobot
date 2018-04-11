#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SerialCommunication.h"
#ifndef BTSERIALCOMMUNICATION_H_
#define BTSERIALCOMMUNICATION_H_

/**
 * This object is used to handle a Bluetooth serial connection
 * on the two passed pins.
 */
class BTSerialCommunication : public SerialCommunication {
public:
    /**
     * This is the constructor of the object and creates the connection.
     * @param tx_pin the pin used to send information
     * @param rx_pin the pin used to receive information
     */
    BTSerialCommunication(int tx_pin, int rx_pin);

    void updateBuffer();

    void print(String* string);

private:
    /**
     * This is the baud rate of the Bluetooth channel.
     */
    static const int BR = 9600;

    /**
     * This is the object that allows to send and receive characters.
     */
    SoftwareSerial *bt;
};

#endif