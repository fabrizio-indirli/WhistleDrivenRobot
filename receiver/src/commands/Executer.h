#include <Arduino.h>
#include "../hashmap/IntegerHashMap.h"
#define HASH_MAP_SIZE 100
#define COMMAND_POSITION 0

class Executer{
public:
	Executer();
	
	void execute(String* string);
	
private:
	IntegerHashMap<void (*)(String)> commandsMap = IntegerHashMap<void (*)(String)>(HASH_MAP_SIZE);
};
