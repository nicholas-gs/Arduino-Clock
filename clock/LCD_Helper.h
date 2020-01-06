#pragma once
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "values.h"

/*
	struct to pass temperature & humidity information to LCDModule class for display in LCD screen
*/
typedef struct {
	float temperature;
	float relative_humidity;
} TempHumidityDataPoint;

/*
	struct to pass navigation instructions to alarm menu
*/
typedef struct {
	bool nextChar = false; // Move to next character on screen
	bool nextDigit = false; // Increment to next digit on screen
} AlarmMenuNavigation ;

class LCDModule {
private:
	const static uint8_t address = 0x27;		// I2C address of DS1307 RTC module
	LiquidCrystal_I2C *lcdModule;
	DateTime* alarmTime;
	uint8_t cursorPosition = 0;					// Keep track of cursor position for alarm menu
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
		Draw 2 single line divider in main menu and alarm icon if alarm enabled by user
	*/
	void initMainMenu(bool* status);

	/*
		Update time displayed on main menu
	*/
	void updateMainMenuTime(DateTime* dataTime);

	/*
		Display or erase alarm icon in main menu
	*/
	void toggleAlarmIcon(bool* status);

	/*
		Update temperature & humidity displayed on main menu
	*/
	void updateMainMenuTempHumidity(TempHumidityDataPoint* dataPoint);

	/*
		Display a label showing user that he has entered the alarm menu. 
		Also pass in the pointer to the DateTime object storing the alarm timing
	*/
	void startAlarmMenuScreen(DateTime* dateTime);

	/*
		Allow user to change alarm timing. Method takes in pointer to AlarmMenuNavigation struct which tells
		what navigation button user has pressed while in alarm menu
	*/
	void adjustAlarmTiming(AlarmMenuNavigation* alarmMenuNav);
};