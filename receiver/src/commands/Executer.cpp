#include <Arduino.h>
#include "../hashmap/IntegerHashMap.h"
#include "Executer.h"
#include "filler.h"

Executer::Executer(){
	fillHashMap(this->commandsMap);
}

void Executer::execute(String* string){
	char commandName = string->charAt(COMMAND_POSITION);
	Serial.println(*string);
	if(this->commandsMap.containsKey(commandName)){
		Serial.println("I know");
		(*(this->commandsMap.get(commandName)))(*string);
	}
}
