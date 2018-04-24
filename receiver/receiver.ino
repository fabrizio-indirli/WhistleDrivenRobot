#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#include "src/hashmap/IntegerHashMap.h"
#include "src/commands/Executer.h"
#define BT_TX_PIN 12
#define BT_RX_PIN 11

SerialCommunication* communication;
Executer* executer;

void setup() {
    //communication = new BTSerialCommunication(BT_TX_PIN, BT_RX_PIN);
    communication = new USBSerialCommunication();
    executer = new Executer();
    String string = "I'm ready";
    communication->print(&string);
    pinMode(10, OUTPUT);
}

void loop() {
    communication->updateBuffer();
    if(communication->isTheLineAllRead()){
  	    String* string = communication->getReadLine();
  	    communication->print(string);
  	    pinMode(10, HIGH);
  	    delay(100);
  	    executer->execute(string);
    }
    
   pinMode(10, LOW);
}
