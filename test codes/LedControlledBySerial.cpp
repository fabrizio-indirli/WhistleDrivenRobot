/* 
 * This program allows to control the four led frequencies by the serial port.
 * To change a led frequence use the syntax (the space in the middle is important): [g|o|r|b] [integer > 0]
 * The program has a process and a Controller instance for each led.
 * The class Controller is used to communicate a frequency to the led process in a thread-safe method.
 * It is necessary to follow this connections: PB10 on RX and PB11 on TX.
*/
#include <cstdio>
#include <string>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <miosix.h>
#include <unistd.h>
#define NS_TO_WAIT_FOR_CHANGE 500000
#define ORANGE_LED 12u
#define GREEN_LED 13u
#define RED_LED 14u
#define BLUE_LED 15u
#define LED_NUM 4

using namespace miosix;

class Controller{
	private:
		int f = 1;
		pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
		
	public:
		Controller(int f){
			this->f = f;
		}
		
		Controller(){
			this->f = 1;
		}
	
		void setF(int f){
			if(f <= 0)
				return;
			pthread_mutex_lock(&mutex);
			this->f = f;
			pthread_mutex_unlock(&mutex);
		}
		
		int getF(){
			int toReturn;
			pthread_mutex_lock(&mutex);
			toReturn = f;
			pthread_mutex_unlock(&mutex);
			return toReturn;
		}
};

Controller ledControllers[LED_NUM];

template <unsigned char LED_N>
void *threadFunction(void *arg){
	Controller *controller = (Controller*)arg;
	
	for(;;){
	    Gpio<GPIOD_BASE, LED_N>::high();
	    usleep(NS_TO_WAIT_FOR_CHANGE/(controller->getF()));
	    Gpio<GPIOD_BASE, LED_N>::low();
	    usleep(NS_TO_WAIT_FOR_CHANGE/(controller->getF()));
	}
    
    return NULL;
}

void *serialReader(void *arg){
	std::string message;
	std::cout << "Hi\n";
	for(;;){
		std::getline(std::cin, message);
        std::cout << "I received: " << message << "\n";
        
        int number = atoi(message.substr(message.find(" ")+1, message.length()).c_str());
        
        switch(message.at(0)){
        	case 'g': ledControllers[0].setF(number);
        			break;
        	case 'o': ledControllers[1].setF(number);
        			break;
        	case 'r': ledControllers[2].setF(number);
        			break;
        	case 'b': ledControllers[3].setF(number);
        			break;
        }
	}
	return NULL;
}

int main(){
	int i;
	Gpio<GPIOD_BASE,ORANGE_LED>::mode(Mode::OUTPUT);
    Gpio<GPIOD_BASE,GREEN_LED>::mode(Mode::OUTPUT);
    Gpio<GPIOD_BASE,RED_LED>::mode(Mode::OUTPUT);
    Gpio<GPIOD_BASE,BLUE_LED>::mode(Mode::OUTPUT);
    
	pthread_t threads[LED_NUM+1];
	
    pthread_create(&threads[0], NULL, &threadFunction<ORANGE_LED>, &ledControllers[0]);
    pthread_create(&threads[1], NULL, &threadFunction<GREEN_LED>, &ledControllers[1]);
    pthread_create(&threads[2], NULL, &threadFunction<RED_LED>, &ledControllers[2]);
    pthread_create(&threads[3], NULL, &threadFunction<BLUE_LED>, &ledControllers[3]);
    pthread_create(&threads[4], NULL, &serialReader, NULL);
    
    
    for(i = 0; i<= LED_NUM; i++)
    	pthread_join(threads[i], NULL);

	return 0;
}
