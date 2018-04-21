#include <Arduino.h>
#include "../hashmap/IntegerHashMap.h"
#define MOTOR1_DIR1_N_PIN 2
#define MOTOR1_DIR1_P_PIN 3

#define MOTOR1_DIR2_N_PIN 4
#define MOTOR1_DIR2_P_PIN 5

#define MOTOR2_DIR1_N_PIN 6
#define MOTOR2_DIR1_P_PIN 7

#define MOTOR2_DIR2_N_PIN 8
#define MOTOR2_DIR2_P_PIN 9

#define BUZZER_PIN 13

void fillHashMap(IntegerHashMap<void (*)(String)>& map);

template <int N_TO_ACTIVATE, int P_TO_ACTIVATE, int N_TO_DEACTIVATE, int P_TO_DEACTIVATE>
void activeDirection(String string);

template <int N1, int P1, int N2, int P2>
void deactivateMotors(String string);

template <int N_MOS_PIN, int P_MOS_PIN>
void activate();

template <int N_MOS_PIN, int P_MOS_PIN>
void deactivate();

template <bool TODO>
void buzzerHandler(String string);
