#include "filler.h"
#include "../hashmap/IntegerHashMap.h"

void fillHashMap(IntegerHashMap<void (*)(String)>& map){
	pinMode(MOTOR1_DIR1_PIN, OUTPUT);
	pinMode(MOTOR1_DIR2_PIN, OUTPUT);
	pinMode(MOTOR2_DIR1_PIN, OUTPUT);
	pinMode(MOTOR2_DIR2_PIN, OUTPUT);
	map.put('a', &changePinStatus<MOTOR1_DIR1_PIN, MOTOR1_DIR2_PIN>);
	map.put('b', &changePinStatus<MOTOR1_DIR2_PIN, MOTOR1_DIR1_PIN>);
	map.put('c', &changePinStatus<MOTOR2_DIR1_PIN, MOTOR2_DIR2_PIN>);
	map.put('d', &changePinStatus<MOTOR2_DIR2_PIN, MOTOR2_DIR1_PIN>);
	map.put('e', &stopAll);
}

template <int PIN_TO_ACTIVATE, int PIN_TO_DEACTIVATE>
void changePinStatus(String string){
	Serial.println(PIN_TO_ACTIVATE);
	digitalWrite(PIN_TO_DEACTIVATE, LOW);
	digitalWrite(PIN_TO_ACTIVATE, HIGH);
}

void stopAll(String string){
	digitalWrite(MOTOR1_DIR1_PIN, LOW);
	digitalWrite(MOTOR1_DIR2_PIN, LOW);
	digitalWrite(MOTOR2_DIR1_PIN, LOW);
	digitalWrite(MOTOR2_DIR2_PIN, LOW);
}
