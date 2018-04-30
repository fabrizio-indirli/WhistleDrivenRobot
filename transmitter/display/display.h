#include <../miosix/util/lcd44780.h>
#include "../freq_recognition/freq_recognition.h"

using namespace miosix;

/**
 * These are the pins used to control the display.
 * VSS (pin 1) to GND
 * VDD (pin 2) to 5V
 * V0 (pin 3) to the center tap of a potentiometer with the other two ends connected to 5V and GND
 * RW (pin 5) to GND
 * if present, A (pin 15) to 3.3V and pin K (PIN 16) to GND
 */
typedef Gpio<GPIOE_BASE,11> d4; //d4 pin (pin 11) to PE11
typedef Gpio<GPIOE_BASE,12> d5; //d5 pin (pin 12) to PE12
typedef Gpio<GPIOE_BASE,13> d6; //d6 pin (pin 13) to PE13
typedef Gpio<GPIOE_BASE,14> d7; //d7 pin (pin 14) to PE14
typedef Gpio<GPIOE_BASE,7> rs;  //rs pin (pin 4) to PE7
typedef Gpio<GPIOE_BASE,8> e;   //E pin (pin 6) to PE8

/**
 * This enum is used to define a command within the display.
 */
enum Command{
	FORWARD = 'f',
	BACK = 'b',
	LEFT = 'l',
	RIGHT = 'r',
	STOP = 's',
	NONE = 'n',
	UNDEFINED = 'u'
};

/**
 * This class embeds the real object to communicate with the display, reducing the number of updates required.
 * It exposes some methods able to updare the frequency and the command shown.
 */
class Display{
public:
	Display();
	
	void setFrequencyTooLow();
	
	void setFrequency(float32_t frequency);
	
	void setCommand(Command command);
	
private:
	/**
	 * This is the length of the display in number of characters.
	 */
	static const int LENGTH = 16;
	
	/**
	 * This is the height of the display in number of characters.
	 */
	static const int HEIGHT = 2;
	
	/**
	 * This is the real display object used to communicate with it.
	 */
	Lcd44780* display;
	
	/**
	 * This is the actual command shown on the display.
	 */
	Command command;
	
	/**
	 * If this flag is true, no frequency is shown on the display.
	 */
	bool isFrequencyTooLow;
	
	/**
	 * This is the actual frequency.
	 */
	float32_t frequency;
	
	/**
	 * This is the method that draws the frequency on the display.
	 */
	void updateFrequency();
	
	/**
	 * This is the method that draws the command on the display.
	 */
	void updateCommand();
	
	/**
	 * This method is used to fill the slots excess with blanc spaces in a line.
	 * @param n the number of used spaces in a line
	 */
	void fillDisplaySpace(int n);
};
