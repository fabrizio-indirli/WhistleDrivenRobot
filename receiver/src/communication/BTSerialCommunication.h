#include <SoftwareSerial.h>
#include "SerialCommunication.h"

class BTSerialCommunication : public SerialCommunication {
public:

    BTSerialCommunication(int tx_pin, int rx_pin);

    void updateBuffer();

    void print(String* string);

private:
    static const int BR = 9600;
    SoftwareSerial *bt;
};