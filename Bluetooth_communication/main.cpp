
#include "registers.h"
#include "serial.h"

//PD14: red led

void delay()
{
	volatile int i;
	for(i=0;i<1000000;i++) ;
}

SerialPort serial;

int main()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 1<<28;
	bool led=false;
	for(;;)
	{
		serial.write("1");//sends the 'move forward' command over bluetooth to the receiver
		delay();
		serial.write("2");//sends the 'turn left' command over bluetooth to the receiver
		delay();
		serial.write("3");//sends the 'turn right' command over bluetooth to the receiver
		delay();
		serial.write("4");//sends the 'move backwards' command over bluetooth to the receiver
		delay();
		if(led) GPIOD->BSRR=1<<14;
		else GPIOD->BSRR=1<<(14+16);
		led=!led;
	}
}
