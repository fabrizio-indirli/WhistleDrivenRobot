#include <SoftwareSerial.h>
 
//definisco pin RX e TX da Arduino verso modulo BT
#define BT_TX_PIN 11
#define BT_RX_PIN 12
 
//istanzio oggetto SoftwareSerial (il nostro futuro bluetooth)
SoftwareSerial bt =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
 
void setup() {
 
  //definisco modalità pin
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
 
  //inizializzo comunicazione Seriale
  Serial.begin(9600);
 
  //inizializzo comunicazione Bluetooth
  bt.begin(9600);
 
}
 
void loop() {
  //se ci sono dati sul buffer della Serial
  while (Serial.available() > 0) {
    //mandali al modulo bluetooth
    bt.print(Serial.read());
  }
 
  //se ci sono dati sul buffer SoftwareSerial (il buffer del bluetooth)
  while (bt.available() > 0) {
    //mostrali nel Serial Monitor
    Serial.println(bt.read());
  }
}
