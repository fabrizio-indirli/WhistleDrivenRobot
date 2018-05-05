#include <SoftwareSerial.h>
 
//definisco pin RX e TX da Arduino verso modulo BT
#define BT_TX_PIN 12
#define BT_RX_PIN 11

#define MOTOR1_DIR1_N_PIN 2
#define MOTOR1_DIR1_P_PIN 3

#define MOTOR1_DIR2_N_PIN 4
#define MOTOR1_DIR2_P_PIN 5

#define MOTOR2_DIR1_N_PIN 6
#define MOTOR2_DIR1_P_PIN 7

#define MOTOR2_DIR2_N_PIN 8
#define MOTOR2_DIR2_P_PIN 9
 
//istanzio oggetto SoftwareSerial (il nostro futuro bluetooth)
SoftwareSerial bt =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
char command;

void activate(int nmos_pin, int pmos_pin)
{
  digitalWrite(nmos_pin, HIGH);
  digitalWrite(pmos_pin, LOW);
}

void deactivate(int nmos_pin, int pmos_pin)
{
  digitalWrite(nmos_pin, LOW);
  digitalWrite(pmos_pin, HIGH);
}


void deactivateMotors(int N1, int P1, int N2, int P2){
  deactivate(N1, P1);
  deactivate(N2, P2);
}

void activeDirection(int N_TO_ACTIVATE, int P_TO_ACTIVATE, int N_TO_DEACTIVATE, int P_TO_DEACTIVATE){
  deactivate(N_TO_DEACTIVATE, P_TO_DEACTIVATE);
  activate(N_TO_ACTIVATE, P_TO_ACTIVATE);
}

void setup() {
 
  //definisco modalità pin
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
 
  //inizializzo comunicazione Seriale
  Serial.begin(9600);
 
  //inizializzo comunicazione Bluetooth
  bt.begin(9600);

  pinMode(MOTOR1_DIR1_N_PIN, OUTPUT);
  pinMode(MOTOR1_DIR1_P_PIN, OUTPUT);
  deactivate(MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN);
   
  pinMode(MOTOR1_DIR2_N_PIN, OUTPUT);
  pinMode(MOTOR1_DIR2_P_PIN, OUTPUT);
  deactivate(MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN);

  pinMode(MOTOR2_DIR1_N_PIN, OUTPUT);
  pinMode(MOTOR2_DIR1_P_PIN, OUTPUT);
  deactivate(MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN);

  pinMode(MOTOR2_DIR2_N_PIN, OUTPUT);
  pinMode(MOTOR2_DIR2_P_PIN, OUTPUT);
  deactivate(MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN);
}
 
void loop() {
 
  //se ci sono dati sul buffer SoftwareSerial (il buffer del bluetooth)
  while (bt.available() > 0) {
    //mostrali nel Serial Monitor
    command = bt.read();
    switch (command) {
  case 'f':
    Serial.println("Move forward");
    activeDirection(MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN);
    activeDirection(MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN, MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN);
    break;
  case 'l':
    activeDirection(MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN, MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN);
    activeDirection(MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN, MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN);
    Serial.println("Turn left");
    break;
  case 'r':
    activeDirection(MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN);
    activeDirection(MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN, MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN);
    Serial.println("Turn right");
    break;
  case 'b':
    activeDirection(MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN, MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN);
    activeDirection(MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN, MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN);
    Serial.println("Move backward");
    break;
  case 's':
    deactivateMotors(MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN);
    deactivateMotors(MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN, MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN);
    Serial.println("STOP");
    break;
  default:
    //Serial.print(command);
    break;
    
    }
  }
}
