#include <stdio.h>
#include <wiringPi.h>

#define GSCLK 7 // All pins are based on their wiringPi indices
#define XLAT 11
#define BLANK 1
#define SIN 12
#define SCLK 14
#define TLC5941_NUM 3

int main(void) {
        for (;;) {

        }

        return 0;
}

void TLC5941_Init(void) {
        // Start up wiringPi module; prepare to set pins
        wiringPiSetup();
        // Start GSCLK using the internal RPi clock (pin 7 special function)
        pinMode(GSCLK, GPIO_CLOCK);
        gpioClockSet(GSCLK, 2400000);
        // Start BLANK clock/PWM  (pin 1  hardware PWM special function)
        pinMode(BLANK, PWM_OUTPUT);
        pwmSetRange(1024);
        pwmWrite(BLANK, 1);
}