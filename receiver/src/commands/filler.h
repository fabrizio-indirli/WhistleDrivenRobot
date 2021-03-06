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

void fillHashMap(IntegerHashMap<void (*)(String)>& map);

void forward(String string);

void back(String string);

void right(String string);

void left(String string);

void deactivateAll(String string);

template <int N_TO_ACTIVATE, int P_TO_ACTIVATE, int N_TO_DEACTIVATE, int P_TO_DEACTIVATE>
void activeDirection();

template <int N1, int P1, int N2, int P2>
void deactivateMotors();

template <int N_MOS_PIN, int P_MOS_PIN>
void activate();

template <int N_MOS_PIN, int P_MOS_PIN>
void deactivate();
