#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#define BT_TX_PIN 12
#define BT_RX_PIN 11

BTSerialCommunication *btCommunication;
USBSerialCommunication *usbSerialCommunication;

void setup() {
  //Serial.begin(9600);
  btCommunication = new BTSerialCommunication(BT_TX_PIN, BT_RX_PIN);
  usbSerialCommunication = new USBSerialCommunication();
}

void loop() {
  btCommunication->updateBuffer();
  usbSerialCommunication->updateBuffer();
  /*if(btCommunication->isTheLineAllRead())
      usbSerialCommunication->print(btCommunication->getReadLine());
  if(usbSerialCommunication->isTheLineAllRead())
      btCommunication->print(usbSerialCommunication->getReadLine());
*/     

    if(usbSerialCommunication->isTheLineAllRead())
      usbSerialCommunication->print(usbSerialCommunication->getReadLine());
}
