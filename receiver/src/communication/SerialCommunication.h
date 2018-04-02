#include <Arduino.h>
#include "../stringhandler/StringHandler.h"
#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_

class SerialCommunication {
public:
    SerialCommunication();

    virtual void updateBuffer();

    virtual void print(String* string);

    bool isTheLineAllRead();

    String* getReadLine();

protected:
    static const int MAX_BUFFER_LENGTH = 500;
    StringHandler stringHandler;

    void putInTheBuffer(char c);

private:
    static const char END_CHAR = '\n';
};

#endif