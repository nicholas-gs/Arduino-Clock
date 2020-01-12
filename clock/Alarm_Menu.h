#pragma once
#include "LCD_Helper.h"

class AlarmMenu {
private:
	LCDModule* lcdModule;
	DateTime* alarmTime;
	uint8_t cursorPosition = 0;					// Keep track of cursor position for alarm menu
public:

	/*
		Constructor
	*/
	AlarmMenu();

	/*
		Initialise object
	*/
	void init(LCDModule* lcdModule);

	/*
	Display a label showing user that he has entered the alarm menu.
	Also pass in the pointer to the DateTime object storing the alarm timing
	*/
	void startAlarmMenuScreen(DateTime* dateTime);

	/*
		Display the alarm timing in the alarm menu
	*/
	void displayAlarmTime();

	/*
		Allow user to change alarm timing. Method takes in pointer to AlarmMenuNavigation struct which tells
		what navigation button user has pressed while in alarm menu
	*/
	void adjustAlarmTiming(AlarmMenuNavigation* alarmMenuNav);

	/*
		Traverse the alarm menu according to predefined positions
	*/
	void traverseAlarmMenu();
};