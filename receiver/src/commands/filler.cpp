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
	pinMode(BUZZER_PIN, OUTPUT);
	buzzerHandler<false>("");
	
	map.put('a', &activeDirection<MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN>);
	map.put('b', &activeDirection<MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN, MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN>);
	map.put('e', &deactivateMotors<MOTOR1_DIR1_N_PIN, MOTOR1_DIR1_P_PIN, MOTOR1_DIR2_N_PIN, MOTOR1_DIR2_P_PIN>);
	map.put('y', &buzzerHandler<true>);
	map.put('z', &buzzerHandler<false>);
}

template <int N_TO_ACTIVATE, int P_TO_ACTIVATE, int N_TO_DEACTIVATE, int P_TO_DEACTIVATE>
void activeDirection(String string){
	deactivate<N_TO_DEACTIVATE, P_TO_DEACTIVATE>();
	activate<N_TO_ACTIVATE, P_TO_ACTIVATE>();
}

template <int N1, int P1, int N2, int P2>
void deactivateMotors(String string){
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

template <bool TODO>
void buzzerHandler(String string){
	if(TODO)
		digitalWrite(BUZZER_PIN, HIGH);
	else
		digitalWrite(BUZZER_PIN, LOW);
}
