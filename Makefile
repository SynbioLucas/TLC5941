# Makefile for the TLC5941 driver library
all: TLC5941
TLC5941: TLC5941.c
	gcc -Wall -o /home/pi/TLC5941_bin TLC5941.c -lwiringPi
