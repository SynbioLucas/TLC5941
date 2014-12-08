#include <stdio.h>
#include <wiringPi.h>

// wiringPi pin 1 is the hardware PWM pin
//#define wpPin 1
#define wpPin 7
#define wpPin2 1

int main(void) {
	printf("Starting up GSCLK...\n");
	wiringPiSetup();
	pinMode(wpPin, GPIO_CLOCK);
	//pwmWrite(wpPin, 128);
	gpioClockSet(wpPin, 1200000);
	pinMode(wpPin2, PWM_OUTPUT);
	pwmSetRange(4096);
	pwmWrite(wpPin2, 1);
	return 0;
}
