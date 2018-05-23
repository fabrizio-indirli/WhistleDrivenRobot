#include "src/commands/Executer.h"
/**
 * If this define is enabled, the compiler will create a code that executes
 * the received commands without expecting parameters. This choice increase
 * the performarces but forbid any parameters sending in the Bluetooth channel.
 */
#define OPTIMIZATION 1
#ifndef OPTIMIZATION
#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#else
#include "src/optimized-communication/optimized-communication.h"
#endif

#ifndef OPTIMIZATION
SerialCommunication* communication;
String* string;
#else
OptimizedCommunication optimized_communication;
#endif
Executer* executer;

void setup() {
#ifndef OPTIMIZATION
    communication = new BTSerialCommunication();
#endif
    executer = new Executer();
}

void loop() {
#ifndef OPTIMIZATION
    communication->updateBuffer();
    if(communication->isTheLineAllRead()){
  	    string = communication->getReadLine();
  	    Serial.println(*string);
  	    executer->execute(string);
    }
#else
    if(optimized_communication.doYouHaveCharacters()){
        char c = optimized_communication.read();
        if(isALowerCaseLiteralChar(c))
            executer->optimizedExecute(c);
    }
#endif
}
