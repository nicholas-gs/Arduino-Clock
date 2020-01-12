#pragma once
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "values.h"

/*
	Wrapper for the LiquidCrystal class; Probably unnecessary but whatever...
*/
class LCDModule {
private:
	const static uint8_t address = 0x27;		// I2C address of DS1307 RTC module
	LiquidCrystal_I2C *liquidCrystal;
public:

	/*
		Empty constructor
	*/
	LCDModule();

	/*
		Initialise class
	*/
	void start();

	/*
		Clear LCD screen
	*/
	void clearScreen();

	/*
		Move the LCD cursor to the position indicated by cursorPosition class attribute
	*/
	void gotoCursorPosition(uint8_t column, uint8_t row);

	/*
		Turn off blinking effect of cursor
	*/
	void noBlink();

	/*
		Turn on blinking effect of cursor
	*/
	void blinkOn();

	/*
		Set cursor position for the 16x2 LCD screen. If invalid cursor positions are entered, default position of (0,0) is set
	*/
	void setCursor(uint8_t row, uint8_t column);

	/*
		Print string from PROGMEM
	*/
	void print(const __FlashStringHelper* c);

	/*
		Print string by passing in char pointer
	*/
	void print(const char* c);

	/*
		Display or erase alarm icon in main menu
	*/
	void displayAlarmIcon(bool status, uint8_t column, uint8_t row);

	/*
		Create custom character
	*/
	void createChar(uint8_t index, const char* charmap);

	/*
		Print custom character
	*/
	void write(uint8_t index);

};