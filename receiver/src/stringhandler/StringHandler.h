#include <Arduino.h>
#ifndef STRINGHANDLER_H_
#define STRINGHANDLER_H_

class StringHandler {
public:
    StringHandler();

    void addChar(char ch);

    String* getString();

    bool areYouOver();

private:
    static const int STRING_LENGTH = 100;
    static const char END_CHAR = '\n';
    char buffer[STRING_LENGTH+1];
    int bufferIndex;
    bool isOver;
};

#endif
