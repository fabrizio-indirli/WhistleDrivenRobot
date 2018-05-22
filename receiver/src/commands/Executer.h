#include <Arduino.h>
#include "../hashmap/IntegerHashMap.h"
/**
 * This is the maximum number of elements in the Hash map.
 */
#define HASH_MAP_SIZE 50
/**
 * This is the position in the command string where its name is placed.
 */
#define COMMAND_POSITION 0
#ifndef EXECUTER_H_
#define EXECUTER_H_

/**
 * This class is used to execute a command received through a serial channel.
 * It uses an Hash map able to get, given the command name, a function pointer
 * to the right instructions to execute. For simplification, the command name
 * has to be only a single character.
 */
class Executer{
public:
	Executer();

	/**
	 * This is the method that gets the right function pointer from the map
	 * and executes it.
	 * @param string the command received
	 */
	void execute(String* string);

	/**
	 * This method is used to optimize the function lookup and its execution.
	 * It takes as parameter the name of the function and it will invoke it
	 * without passing the received string as parameter.
	 * @param c
	 */
	void optimizedExecute(char c);
	
private:
	/**
	 * This is the optimization structure that allows to get the function
	 * pointer in a constant time.
	 */
	IntegerHashMap<void (*)(String)> commandsMap = IntegerHashMap<void (*)(String)>(HASH_MAP_SIZE);
};

#endif
