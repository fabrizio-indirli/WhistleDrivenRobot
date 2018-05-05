#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#include "src/hashmap/IntegerHashMap.h"
#include "src/commands/Executer.h"

SerialCommunication* communication;
Executer* executer;
String* string;

void setup() {
    communication = new BTSerialCommunication();
    //communication = new USBSerialCommunication();
    executer = new Executer();
    Serial.begin(9600);
    Serial.println("ciao");
}

void loop() {
    communication->updateBuffer();
    if(communication->isTheLineAllRead()){
  	    string = communication->getReadLine();
  	    Serial.println(*string);
  	    executer->execute(string);
    }
}
