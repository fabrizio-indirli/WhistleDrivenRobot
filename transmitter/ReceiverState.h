#include <miosix.h>
#include <display/display.h>

/**
 * This class allows to send commands to the receiver.
 * To avoid an overwhelming stream of strings, this object doesn't send 
 * several times the same commands, broadcasting only the first element
 * of a sequence made by the same repeated string.
 * This object also avoid to send senseless string to the receiver.
 */
class ReceiverState{
public:
	ReceiverState(){
		actualCommand = UNDEFINED;
	}
	
	/**
	 * This is the method to invoke for sending a command, represented by the passed enum value.
	 * This method updates the current state of the object and, if it is changed,
	 * provides to send the new command to the receiver.
	 */
	void setState(Command command){
		if(actualCommand == command)
			return;
			
		actualCommand = command;
		sendCommand();
	}

private:
	/**
	 * This is the actual state of the receiver.
	 */
	Command actualCommand;
	
	/**
	 * This method sends the command to the receiver, regardless if it is already sent or less.
	 * (It can be optimize through an Hash map.)
	 */
	void sendCommand(){
		switch(actualCommand){
			case FORWARD:	printf("f\n");
							break;
			case BACK:		printf("b\n");
							break;
			case LEFT:		printf("l\n");
							break;
			case RIGHT:		printf("r\n");
							break;
			case STOP:		
			case NONE:
			case UNDEFINED:	printf("s\n");
							break;
		}
	}
};
