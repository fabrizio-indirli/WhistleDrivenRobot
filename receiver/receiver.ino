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
    //btCommunication = new BTSerialCommunication(BT_TX_PIN, BT_RX_PIN);
    usbSerialCommunication = new USBSerialCommunication();
    executer = new Executer();
    String message = "Hi, I'm ready to take commands";
    usbSerialCommunication->print(&message);
}

void loop() {
    usbSerialCommunication->updateBuffer();

    if(usbSerialCommunication->isTheLineAllRead()){
  	    String* string = usbSerialCommunication->getReadLine();
  	    usbSerialCommunication->print(string);
  	    executer->execute(string);
    }
}
