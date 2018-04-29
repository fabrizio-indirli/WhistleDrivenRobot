#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#include "src/hashmap/IntegerHashMap.h"
#include "src/commands/Executer.h"

SerialCommunication* communication;
Executer* executer;

void setup() {
    //communication = new BTSerialCommunication();
    communication = new USBSerialCommunication();
    executer = new Executer();
    Serial.begin(9600);
}

void loop() {
    communication->updateBuffer();
    if(communication->isTheLineAllRead()){
  	    String* string = communication->getReadLine();
  	    executer->execute(string);
    }
}
