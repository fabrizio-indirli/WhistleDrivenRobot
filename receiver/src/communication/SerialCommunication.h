#include <Arduino.h>
#include "../stringhandler/StringHandler.h"
#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_

/**
 * This abstract class handles a generic serial data source.
 * This object takes characters from the serial until it receives a termination character
 * and, when this happens, the receiving process is complete. Consequently, it waits
 * for the string withdrawal before restarting the taking process. In a given moment,
 * only a building string can be stored in this object.
 * This object allows also to transmit strings on the same channel.
 */
class SerialCommunication {
public:
    SerialCommunication();

    /**
     * This method is used to take the available character for the serial
     * and to store them in this object.
     */
    virtual void updateBuffer();

    /**
     * This method is used to send a string on the serial channel.
     * @param string the string to send
     */
    virtual void print(String* string);

    /**
     * This method returns true if a termination character is received and,
     * consequently, the received string can be taken.
     * @return true if the complete string is available, else false
     */
    bool isTheLineAllRead();

    /**
     * This method is used to remove the just read string, returning it
     * and letting the serial reading process start again.
     * @return the just read string
     */
    String* getReadLine();

protected:
    /**
     * This is the method used to store the building string.
     * It recognizes by itself when a string is complete.
     */
    StringHandler stringHandler;

    /**
     * This method is used to add a single character to the building string.
     * @param c
     */
    void putInTheBuffer(char c);
};

#endif
