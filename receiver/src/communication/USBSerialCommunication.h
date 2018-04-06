#include <Arduino.h>
#include "SerialCommunication.h"

class USBSerialCommunication : public SerialCommunication {
public:

    USBSerialCommunication();

    void updateBuffer();

    void print(String* string);

private:
    static const int BR = 9600;
};
