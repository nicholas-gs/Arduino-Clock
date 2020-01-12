#include "Alarm_Menu.h"

AlarmMenu::AlarmMenu() {

}

void AlarmMenu::init(LCDModule* lcdModule) {
	AlarmMenu::lcdModule = lcdModule;
}

void AlarmMenu::startAlarmMenuScreen(DateTime* dateTime) {
	AlarmMenu::alarmTime = dateTime; // Assign alarm timing object pointer
	AlarmMenu::cursorPosition = 0;  // Reset cursor position

	AlarmMenu::lcdModule->setCursor(3, 0);
	AlarmMenu::lcdModule->print(F("Alarm Menu"));
	delay(1000);  // Display "Alarm Menu" for 1 second
	AlarmMenu::lcdModule->clearScreen();

	AlarmMenu::lcdModule->setCursor(0, 0);
	AlarmMenu::lcdModule->print(F("Time:"));
	AlarmMenu::lcdModule->setCursor(0, 1);
	AlarmMenu::lcdModule->print(F("Date:"));

	AlarmMenu::displayAlarmTime();
	AlarmMenu::lcdModule->setCursor(5, 0);
	AlarmMenu::lcdModule->blinkOn();

}

void AlarmMenu::displayAlarmTime() {
	char buffer[12];
	// HH:MM of alarm time
	AlarmMenu::lcdModule->setCursor(5, 0);
	uint8_t h = AlarmMenu::alarmTime->hour();
	uint8_t min = AlarmMenu::alarmTime->minute();
	sprintf(buffer, "%02d:%02d", h, min);
	AlarmMenu::lcdModule->print(buffer);

	// DD/MM/YYYY of alarm time
	AlarmMenu::lcdModule->setCursor(5, 1);
	uint8_t d = AlarmMenu::alarmTime->day();
	uint8_t m = AlarmMenu::alarmTime->month();
	uint16_t y = AlarmMenu::alarmTime->year();
	sprintf(buffer, "%02d/%02d/%d", d, m, y);
	AlarmMenu::lcdModule->print(buffer);
}

void AlarmMenu::adjustAlarmTiming(AlarmMenuNavigation* alarmMenuNav) {
	if (alarmMenuNav->nextChar) {
		AlarmMenu::cursorPosition += 1; // Increment cursorPosition index
		AlarmMenu::cursorPosition = AlarmMenu::cursorPosition % 10;
		traverseAlarmMenu();
	}

	if (alarmMenuNav->nextDigit) {
		// Which digit we increment will depend on the cursor position
		uint16_t lastDigit;
		uint16_t firstDigit;
		uint16_t newVal;
		switch (AlarmMenu::cursorPosition)
		{
		case 0: // Increment hour first digit (0, 1, or 2).
			firstDigit = AlarmMenu::alarmTime->hour() / 10;
			lastDigit = AlarmMenu::alarmTime->hour() % 10;
			firstDigit = (firstDigit + 1) % 3;
			newVal = (firstDigit * 10) + lastDigit;	// Get new hour
			AlarmMenu::alarmTime->setHour(newVal);
			break;
		case 1:	// Increment hour second digit ( 0 - 9 if first digit is 0 or 1 ; 0 - 4 if first digit is 2 )
			firstDigit = AlarmMenu::alarmTime->hour() / 10;
			lastDigit = AlarmMenu::alarmTime->hour() % 10;
			if (firstDigit == 0 || firstDigit == 1) {
				lastDigit = (lastDigit + 1) % 10;
			}
			else {	// Digit 2
				lastDigit = (lastDigit + 1) % 5;
			}
			newVal = (firstDigit * 10) + lastDigit; // Get new hour
			AlarmMenu::alarmTime->setHour(newVal);
			break;
		case 2: // Increment minutes first digit (0,1,2,3,4,5).
			firstDigit = AlarmMenu::alarmTime->minute() / 10;
			lastDigit = AlarmMenu::alarmTime->minute() % 10;
			firstDigit = (firstDigit + 1) % 6;
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setMinute(newVal);
			break;
		case 3:	// Increment minutes second digit (0,1,2,3,4,5,6,7,8,9)
			firstDigit = AlarmMenu::alarmTime->minute() / 10;
			lastDigit = AlarmMenu::alarmTime->minute() % 10;
			lastDigit = (lastDigit + 1) % 10;
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setMinute(newVal);
			break;
		case 4:	// Increment date first digit (0,1,2,3)
			firstDigit = AlarmMenu::alarmTime->day() / 10;
			lastDigit = AlarmMenu::alarmTime->day() % 10;
			firstDigit = (firstDigit + 1) % 4;
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setDay(newVal);
			break;
		case 5:	// Increment date last digit (0,1,2,3,4,5,6,7,8,9)
			firstDigit = AlarmMenu::alarmTime->day() / 10;
			lastDigit = AlarmMenu::alarmTime->day() % 10;
			lastDigit = (lastDigit + 1) % 10;
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setDay(newVal);
			break;
		case 6:	// Increment month first digit (0,1)
			firstDigit = AlarmMenu::alarmTime->month() / 10;
			lastDigit = AlarmMenu::alarmTime->month() % 10;
			firstDigit = (firstDigit + 1) % 2;
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setMonth(newVal);
			break;
		case 7:	// Increment month last digit (0 - 9 if first digit is 0, 0 - 2 if last digit is 1 )
			firstDigit = AlarmMenu::alarmTime->month() / 10;
			lastDigit = AlarmMenu::alarmTime->month() % 10;
			if (firstDigit == 0) {
				lastDigit = (lastDigit + 1) % 10;
			}
			else if (firstDigit == 1) {
				lastDigit = (lastDigit + 1) % 3;
			}
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setMonth(newVal);
			break;
		case 8:	// Increment year (offset from 2000) first digit (0,1,2,3,4,5,6,7,8,9)
			firstDigit = (AlarmMenu::alarmTime->year() - 2000) / 10;
			lastDigit = (AlarmMenu::alarmTime->year() - 2000) % 10;
			firstDigit = (firstDigit + 1) % 10;
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setYear(2000 + newVal);
			break;
		case 9:	// Increment year (offset from 2000) last digit (0,1,2,3,4,5,6,7,8,9)
			firstDigit = (AlarmMenu::alarmTime->year() - 2000) / 10;
			lastDigit = (AlarmMenu::alarmTime->year() - 2000) % 10;
			lastDigit = (lastDigit + 1) % 10;
			newVal = (firstDigit * 10) + lastDigit;
			AlarmMenu::alarmTime->setYear(2000 + newVal);
			break;
		}
		AlarmMenu::displayAlarmTime();
		AlarmMenu::traverseAlarmMenu();
	}

	// Reset values
	alarmMenuNav->nextChar = false;
	alarmMenuNav->nextDigit = false;
}

void AlarmMenu::traverseAlarmMenu() {
	const uint8_t* arr = CURSOR_POSITION[AlarmMenu::cursorPosition];
	AlarmMenu::lcdModule->setCursor(arr[0], arr[1]);
}