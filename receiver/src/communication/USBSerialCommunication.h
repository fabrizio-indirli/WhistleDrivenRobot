#include <Arduino.h>
#include "SerialCommunication.h"

/**
 * This object is used to handle a USB serial connection.
 * This class doesn't need any proper objects since all the require information
 * is on the Serial object of Arduino. This class exists only to standardize
 * the connection with the Bluetooth one.
 */
class USBSerialCommunication : public SerialCommunication {
public:

    USBSerialCommunication();

    void updateBuffer();

    void print(String* string);

private:
    /**
     * This is the baud rate of the serial channel.
     */
    static const int BR = 9600;
};
