#include <Arduino.h>
#include <SoftwareSerial.h>
#define BT_TX_PIN 12
#define BT_RX_PIN 11

class OptimizedCommunication{
public:
	OptimizedCommunication();
	
	bool doYouHaveCharacters();
	
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

bool isALowerCaseLitteralChar(char c);
