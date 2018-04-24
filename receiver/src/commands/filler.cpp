#include <Arduino.h>
#include "filler.h"
#include "../hashmap/IntegerHashMap.h"

void fillHashMap(IntegerHashMap<void (*)(String)>& map){
	pinMode(MOTOR1_DIR1_N_PIN, OUTPUT);
	pinMode(MOTOR1_DIR1_P_PIN, OUTPUT);
	deactivate<MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN>();
	pinMode(MOTOR1_DIR2_N_PIN, OUTPUT);
	pinMode(MOTOR1_DIR2_P_PIN, OUTPUT);
	deactivate<MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN>();
	
	pinMode(MOTOR2_DIR1_N_PIN, OUTPUT);
	pinMode(MOTOR2_DIR1_P_PIN, OUTPUT);
	deactivate<MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN>();
	pinMode(MOTOR2_DIR2_N_PIN, OUTPUT);
	pinMode(MOTOR2_DIR2_P_PIN, OUTPUT);
	deactivate<MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN>();
	
	map.put('f', &forward);
	map.put('b', &back);
	map.put('r', &right);
	map.put('l', &left);
	map.put('s', &deactivateAll);
}

void forward(String string){
	activeDirection<MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN>();
	activeDirection<MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN, MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN>();
}

void back(String string){
	activeDirection<MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN, MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN>();
	activeDirection<MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN, MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN>();
}

void right(String string){
	activeDirection<MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN>();
	activeDirection<MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN, MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN>();
}

void left(String string){
	activeDirection<MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN, MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN>();
	activeDirection<MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN, MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN>();
}

void deactivateAll(String string){
	deactivateMotors<MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN>();
	deactivateMotors<MOTOR2_DIR1_N_PIN, MOTOR2_DIR1_P_PIN, MOTOR2_DIR2_N_PIN, MOTOR2_DIR2_P_PIN>();
}

template <int N_TO_ACTIVATE, int P_TO_ACTIVATE, int N_TO_DEACTIVATE, int P_TO_DEACTIVATE>
void activeDirection(){
	deactivate<N_TO_DEACTIVATE, P_TO_DEACTIVATE>();
	activate<N_TO_ACTIVATE, P_TO_ACTIVATE>();
}

template <int N1, int P1, int N2, int P2>
void deactivateMotors(){
	deactivate<N1, P1>();
	deactivate<N2, P2>();
}

template <int N_MOS_PIN, int P_MOS_PIN>
void activate(){
	digitalWrite(N_MOS_PIN, HIGH);
	digitalWrite(P_MOS_PIN, LOW);
}

template <int N_MOS_PIN, int P_MOS_PIN>
void deactivate(){
	digitalWrite(N_MOS_PIN, LOW);
	digitalWrite(P_MOS_PIN, HIGH);
}
