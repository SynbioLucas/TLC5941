///////
// TLC5941.c Test Implementation
// Firmware for Controlling the TLC5941 from a Raspberry Pi.
// Lucas Hartsough
// lucash@rice.edu
//
// Code heavily based on the amazing reference "Demystifying the TLC5940" by Matthew Pandina (artcfox@gmail.com).
// 	The library can be found at: https://projects.drogon.net/raspberry-pi/wiringPi
//
// Note: Board currently has no MODE (VPRG) out pin, so that functionality has not been programmed here.
// Note: The TLC5941 has no DCPRG pin (it's now TEST and is wired to VCC), so all functionality there is ignored.
//
// Requires:
// wiringPi.h -- WiringPi library (wiringPi.com)
///////

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define GSCLK 7 // All pins are based on their wiringPi indices
#define XLAT 11
#define BLANK 1
#define SIN 12
#define SCLK 14
#define TLC5941_N 3

#define pulse(pin) do { \
			digitalWrite(pin, HIGH); \
			digitalWrite(pin, LOW); \
			} while (0)

unsigned int gsData[192 * TLC5941_N] = {
	//MSB				LSB
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 31
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 30
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 29
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 28
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 27
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 26
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 25
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 24
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 23
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 22
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 21
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 20
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 19
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 18
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 17
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 16
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 15
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 14
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 13
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,	// Channel 12
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,	// Channel 11
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,	// Channel 10
	1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,	// Channel 9
	1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,	// Channel 8
	1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,	// Channel 7
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,	// Channel 6
	1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,	// Channel 5
	1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 4
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 3
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 2
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// Channel 1
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	// Channel 0
};

void TLC5941_Init(void);
void TLC5941_SetGS_And_GS_PWM(void);

int main(void) {
        TLC5941_Init();
	for (;;) {
		TLC5941_SetGS_And_GS_PWM();
        }
        return 0;
}

void TLC5941_Init(void) {
        // Start up wiringPi module; prepare to set pins
        wiringPiSetup();
	// Configure Pins
	pinMode(GSCLK, GPIO_CLOCK);
	pinMode(XLAT, OUTPUT);
	pinMode(BLANK, PWM_OUTPUT);
	pinMode(SIN, OUTPUT);
	pinMode(SCLK, OUTPUT);
	// Initialize pins
	digitalWrite(GSCLK, LOW);
	digitalWrite(SCLK, LOW);
	digitalWrite(XLAT, LOW);
	digitalWrite(BLANK, HIGH);
	// Initialize clocks
        // Start GSCLK using the internal RPi clock (pin 7 special function)
	gpioClockSet(GSCLK, 2400000);
        // Start BLANK clock/PWM  (pin 1  hardware PWM special function)
	pwmSetRange(1024);
	pwmWrite(BLANK, 1);
}

void TLC5941_SetGS_And_GS_PWM(void) {
	unsigned int GSCLK_Counter = 0;
	unsigned int Data_Counter = 0;
	
	digitalWrite(BLANK, LOW);
	
	for (;;) {
		if (GSCLK_Counter > 4095) {
			digitalWrite(BLANK, HIGH);
			pulse(XLAT);
			break;
		} else {
			if (!(Data_Counter > TLC5941_N * 192 - 1)) {
				if (gsData[Data_Counter]) {
					digitalWrite(SIN, HIGH);
				} else {
					digitalWrite(SIN, LOW);
				}
				pulse(SCLK);
				Data_Counter += 1;
			}
		}
		pulse(GSCLK);
		GSCLK_Counter += 1;
	}
}
