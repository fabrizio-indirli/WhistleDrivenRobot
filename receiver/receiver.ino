#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#include "src/optimized-communication/optimized-communication.h"
#include "src/hashmap/IntegerHashMap.h"
#include "src/commands/Executer.h"

//SerialCommunication* communication;
OptimizedCommunication opt_c;
Executer* executer;
String* string;

void setup() {
    //communication = new BTSerialCommunication();
    //communication = new USBSerialCommunication();
    executer = new Executer();
}

void loop() {
    /*communication->updateBuffer();
    if(communication->isTheLineAllRead()){
  	    string = communication->getReadLine();
  	    Serial.println(*string);
  	    executer->execute(string);
    }*/
    
    if(opt_c.doYouHaveCharacters()){
    	char c = opt_c.read();
    	if(isALowerCaseLiteralChar(c))
    		executer->optimizedExecute(c);
    }
}
