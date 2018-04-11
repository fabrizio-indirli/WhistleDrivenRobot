#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#include "src/hashmap/IntegerHashMap.h"
#include "src/commands/Executer.h"
#define BT_TX_PIN 12
#define BT_RX_PIN 11

BTSerialCommunication* btCommunication;
USBSerialCommunication* usbSerialCommunication;
Executer* executer;

void setup() {
    btCommunication = new BTSerialCommunication(BT_TX_PIN, BT_RX_PIN);
    //usbSerialCommunication = new USBSerialCommunication();
    executer = new Executer();
}

void loop() {
    btCommunication->updateBuffer();

    if(btCommunication->isTheLineAllRead()){
  	    String* string = btCommunication->getReadLine();
  	    btCommunication->print(string);
  	    executer->execute(string);
    }
}
