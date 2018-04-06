#include "src/communication/BTSerialCommunication.h"
#include "src/communication/USBSerialCommunication.h"
#include "src/hashmap/IntegerHashMap.h"
#define BT_TX_PIN 12
#define BT_RX_PIN 11
#define HASH_MAP_SIZE 100

BTSerialCommunication *btCommunication;
USBSerialCommunication *usbSerialCommunication;

void setup() {
  btCommunication = new BTSerialCommunication(BT_TX_PIN, BT_RX_PIN);
  usbSerialCommunication = new USBSerialCommunication();

  IntegerHashMap<void (*)(String)> commandsMap(HASH_MAP_SIZE);
}

void loop() {
  btCommunication->updateBuffer();
  usbSerialCommunication->updateBuffer();
  if(btCommunication->isTheLineAllRead())
      usbSerialCommunication->print(btCommunication->getReadLine());
  if(usbSerialCommunication->isTheLineAllRead())
      btCommunication->print(usbSerialCommunication->getReadLine());        
}
