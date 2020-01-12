#pragma once
#include "LCD_Helper.h"

/*
	This class represents the main menu of the program that is displayed on the LCD screen.
	It has a class pointer to the LCD module object so that it can display on the screen
*/
class MainMenu {

private:
	LCDModule* lcdModule;

public:

	MainMenu();

	/*
		Initialise object
	*/
	void init(LCDModule* lcdModule);

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
};