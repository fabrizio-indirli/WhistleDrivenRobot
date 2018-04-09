#include <Arduino.h>
#ifndef STRINGHANDLER_H_
#define STRINGHANDLER_H_

/**
 * This class is used to build a string from a serial sequence of characters.
 * It is able to recognize by itself the conditions that implies the
 * string termination (end character and buffer overflow risk).
 */
class StringHandler {
public:
    StringHandler();

    /**
     * This is the method used to add a character.
     * If the string is completed, this one will be ignored.
     * @param ch the character to add
     */
    void addChar(char ch);

    /**
     * This method is used to get the read string without caring
     * if it is ended or not. After this process, the buffer is reset.
     * @return the stored string
     */
    String* getString();

    /**
     * This method is used to know if the read process is over.
     * @return true if it is over, else false
     */
    bool areYouOver();

private:
    /**
     * This is the maximum characters number allowed for a string.
     */
    static const int STRING_LENGTH = 100;

    /**
     * This is the character that defines the string end.
     */
    static const char END_CHAR = '\n';

    /**
     * This is the provisional character storing place where they reside
     * before being extracted.
     */
    char buffer[STRING_LENGTH+1];

    /**
     * This is the actual index of the first unused cell in the buffer.
     */
    int bufferIndex;

    /**
     * This flag defines if the string is all read or not.
     */
    bool isOver;
};

#endif
