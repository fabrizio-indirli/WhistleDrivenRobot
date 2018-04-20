
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
		serial.write("Hello world\r\n");
		delay();
		if(led) GPIOD->BSRR=1<<14;
		else GPIOD->BSRR=1<<(14+16);
		led=!led;
	}
}
