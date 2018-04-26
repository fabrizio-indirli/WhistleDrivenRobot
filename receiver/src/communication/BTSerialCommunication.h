#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SerialCommunication.h"
#ifndef BTSERIALCOMMUNICATION_H_
#define BTSERIALCOMMUNICATION_H_
#define BT_TX_PIN 12
#define BT_RX_PIN 11

/**
 * This object is used to handle a Bluetooth serial connection
 * on the two passed pins.
 */
class BTSerialCommunication : public SerialCommunication {
public:
    /**
     * This is the constructor of the object and creates the connection.
     */
    BTSerialCommunication();

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
    SoftwareSerial bt = SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
};

#endif
