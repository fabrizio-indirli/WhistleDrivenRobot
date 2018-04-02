#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#define BT_TX_PIN 12
#define BT_RX_PIN 11

BTSerialCommunication btCommunication(BT_TX_PIN, BT_RX_PIN);
USBSerialCommunication usbSerialCommunication;

void setup() {
  //TODO
}

void loop() {
  btCommunication.updateBuffer();
  usbSerialCommunication.updateBuffer();


  if(btCommunication.isTheLineAllRead())
      usbSerialCommunication.print(btCommunication.getReadLine());
  if(usbSerialCommunication.isTheLineAllRead())
      btCommunication.print(usbSerialCommunication.getReadLine());
}
