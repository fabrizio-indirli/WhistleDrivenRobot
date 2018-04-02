#include <SoftwareSerial.h>
#include "SerialCommunication.h"

class BTSerialCommunication : public SerialCommunication {
public:

    BTSerialCommunication(int tx_pin, int rx_pin);

    void updateBuffer();

    bool isTheBufferAllRead();

    char* getReadBuffer();

private:
    SoftwareSerial *bt;
};