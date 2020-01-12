#include "Main_Menu.h"

MainMenu::MainMenu() {

}

void MainMenu::init(LCDModule* lcdModule) {
	MainMenu::lcdModule = lcdModule;
}

void MainMenu::initMainMenu(bool* status) {
	MainMenu::lcdModule->noBlink();
	MainMenu::lcdModule->setCursor(10, 0);
	MainMenu::lcdModule->print(F("|"));

	MainMenu::lcdModule->setCursor(10, 1);
	MainMenu::lcdModule->print(F("|"));

	MainMenu::toggleAlarmIcon(status);
}

/*
	Update time displayed on main menu
*/
void MainMenu::updateMainMenuTime(DateTime* dateTime) {
	char buffer[10];

	// Update time
	MainMenu::lcdModule->setCursor(0, 0);
	// HH:MM:SS
	sprintf(buffer, "%02d:%02d:%02d", dateTime->hour(), dateTime->minute(), dateTime->second());
	MainMenu::lcdModule->print(buffer);

	// Update date
	MainMenu::lcdModule->setCursor(0, 1);
	MainMenu::lcdModule->print(weekday_names[dateTime->dayOfTheWeek()]);
	// DD/MM
	MainMenu::lcdModule->setCursor(4, 1);
	sprintf(buffer, "%02d/%02d", dateTime->day(), dateTime->month());
	MainMenu::lcdModule->print(buffer);
}

/*
	Display or erase alarm icon in main menu
*/
void MainMenu::toggleAlarmIcon(bool* status) {
	MainMenu::lcdModule->displayAlarmIcon(*status, 9, 0);
}

/*
	Update temperature & humidity displayed on main menu
*/
void MainMenu::updateMainMenuTempHumidity(TempHumidityDataPoint* dataPoint) {
	char buffer1[3];
	char buffer2[4];

	// Temperature
	MainMenu::lcdModule->setCursor(11, 0);
	dtostrf(dataPoint->temperature, 4, 1, buffer1);
	sprintf(buffer2, "%sC", buffer1);
	MainMenu::lcdModule->print(buffer2);

	// Humidity
	MainMenu::lcdModule->setCursor(11, 1);
	dtostrf(dataPoint->relative_humidity, 4, 1, buffer1);
	sprintf(buffer2, "%s", buffer1);
	MainMenu::lcdModule->print(buffer2);

	MainMenu::lcdModule->setCursor(15, 1);
	MainMenu::lcdModule->print("%");
}