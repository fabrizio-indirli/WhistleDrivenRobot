#include <Arduino.h>
#include <SoftwareSerial.h>
#define BT_TX_PIN 12
#define BT_RX_PIN 11

/**
 * This object provides a simple way to interact with a Bluetooth channel,
 * extracting one character per time.
 */
class OptimizedCommunication{
public:
	OptimizedCommunication();

	/**
	 * This method is used to know if there are some character in the buffer.
	 * @return true if there are some character, else false
	 */
	bool doYouHaveCharacters();

	/**
	 * This method is used to get a character from the buffer. This uses a blocking method,
	 * so, if there are no character in the buffer, the execution will be blocked until a character
	 * is received.
	 * @return a character from the input buffer
	 */
	char read();

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

/**
 * This method determines if a character is a lower case literal or not.
 * @param c the character to control
 * @return true if it is, else false
 */
bool isALowerCaseLiteralChar(char c);
