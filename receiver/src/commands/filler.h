#include <Arduino.h>
#include "../hashmap/IntegerHashMap.h"
#define MOTOR1_DIR1_PIN 2
#define MOTOR1_DIR2_PIN 3
#define MOTOR2_DIR1_PIN 4
#define MOTOR2_DIR2_PIN 5

void fillHashMap(IntegerHashMap<void (*)(String)>& map);

template <int PIN_TO_ACTIVATE, int PIN_TO_DEACTIVATE>
void changePinStatus(String string);

void stopAll(String string);
