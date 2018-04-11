#include <Arduino.h>
#include "../hashmap/IntegerHashMap.h"
#include "Executer.h"
#include "filler.h"

Executer::Executer(){
	fillHashMap(this->commandsMap);
}

void Executer::execute(String* string){
	char commandName = string->charAt(COMMAND_POSITION);
	if(this->commandsMap.containsKey(commandName))
		(*(void (*)(String))(this->commandsMap.get(commandName)))(*string);
}
