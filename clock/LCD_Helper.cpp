#include "LCD_Helper.h"

LCDModule::LCDModule() {

}

void LCDModule::start() {
	LCDModule::lcdModule = new LiquidCrystal_I2C(LCDModule::address, 16, 2);
	LCDModule::lcdModule->init();
	LCDModule::lcdModule->backlight();
}

void LCDModule::clearScreen() {
	LCDModule::lcdModule->clear();
}

void LCDModule::gotoCursorPosition() {
	const uint8_t* arr = CURSOR_POSITION[LCDModule::cursorPosition];
	LCDModule::lcdModule->setCursor(arr[0], arr[1]);
}

void LCDModule::initMainMenu(bool* status) {
	LCDModule::lcdModule->noBlink();
	LCDModule::lcdModule->setCursor(10, 0);
	LCDModule::lcdModule->print(F("|"));

	LCDModule::lcdModule->setCursor(10, 1);
	LCDModule::lcdModule->print(F("|"));

	LCDModule::toggleAlarmIcon(status);
}

void LCDModule::updateMainMenuTime(DateTime* dateTime) {
	char buffer[10];

	// Update time
	LCDModule::lcdModule->setCursor(0,0);
	// HH:MM:SS
	sprintf(buffer, "%02d:%02d:%02d", dateTime->hour(), dateTime->minute(), dateTime->second());
	LCDModule::lcdModule->print(buffer);

	// Update date
	LCDModule::lcdModule->setCursor(0, 1);
	LCDModule::lcdModule->print(weekday_names[dateTime->dayOfTheWeek()]);
	// DD/MM
	LCDModule::lcdModule->setCursor(4, 1);
	sprintf(buffer, "%02d/%02d", dateTime->day(), dateTime->month());
	LCDModule::lcdModule->print(buffer);
}

void LCDModule::toggleAlarmIcon(bool* status) {
	if (*status) {
		LCDModule::lcdModule->createChar(0, ALARM_ICON);
		LCDModule::lcdModule->setCursor(9, 0);
		LCDModule::lcdModule->write(0);
	}
	else {
		LCDModule::lcdModule->setCursor(9, 0);
		LCDModule::lcdModule->print(" ");
	}
}

void LCDModule::updateMainMenuTempHumidity(TempHumidityDataPoint* dataPoint) {
	char buffer1[3];
	char buffer2[4];

	// Temperature
	LCDModule::lcdModule->setCursor(11, 0);
	dtostrf(dataPoint->temperature, 4, 1, buffer1);
	sprintf(buffer2, "%sC", buffer1);
	LCDModule::lcdModule->print(buffer2);

	// Humidity
	LCDModule::lcdModule->setCursor(11, 1);
	dtostrf(dataPoint->relative_humidity, 4, 1, buffer1);
	sprintf(buffer2, "%s", buffer1);
	LCDModule::lcdModule->print(buffer2);

	LCDModule::lcdModule->setCursor(15, 1);
	LCDModule::lcdModule->print("%");

}

void LCDModule::startAlarmMenuScreen(DateTime* dateTime) {
	LCDModule::alarmTime = dateTime; // Assign alarm timing object pointer
	LCDModule::cursorPosition = 0;  // Reset cursor position

	LCDModule::lcdModule->setCursor(3, 0);
	LCDModule::lcdModule->print(F("Alarm Menu"));
	delay(1000);  // Display "Alarm Menu" for 1 second
	LCDModule::clearScreen();

	LCDModule::lcdModule->setCursor(0, 0);
	LCDModule::lcdModule->print(F("Time:"));
	LCDModule::lcdModule->setCursor(0, 1);
	LCDModule::lcdModule->print(F("Date:"));

	LCDModule::displayAlarmTime();
	LCDModule::lcdModule->setCursor(5, 0);
	LCDModule::lcdModule->blink_on();

}

void LCDModule::displayAlarmTime() {

	char buffer[12];
	// HH:MM of alarm time
	LCDModule::lcdModule->setCursor(5, 0);
	uint8_t h = LCDModule::alarmTime->hour();
	uint8_t min = LCDModule::alarmTime->minute();
	sprintf(buffer, "%02d:%02d", h, min);
	LCDModule::lcdModule->print(buffer);

	// DD/MM/YYYY of alarm time
	LCDModule::lcdModule->setCursor(5, 1);
	uint8_t d = LCDModule::alarmTime->day();
	uint8_t m = LCDModule::alarmTime->month();
	uint16_t y = LCDModule::alarmTime->year();
	sprintf(buffer, "%02d/%02d/%d", d,m,y);
	LCDModule::lcdModule->print(buffer);	
}

void LCDModule::adjustAlarmTiming(AlarmMenuNavigation* alarmMenuNav) {
	if (alarmMenuNav->nextChar) {
		LCDModule::cursorPosition += 1; // Increment cursorPosition index
		LCDModule::cursorPosition = LCDModule::cursorPosition % 10;
		gotoCursorPosition();
	}

	if (alarmMenuNav->nextDigit) {
		// Which digit we increment will depend on the cursor position
		uint16_t lastDigit;
		uint16_t firstDigit;
		uint16_t newVal;
		switch (LCDModule::cursorPosition)
		{
			case 0: // Increment hour first digit (0, 1, or 2).
				firstDigit = LCDModule::alarmTime->hour() / 10;
				lastDigit = LCDModule::alarmTime->hour() % 10;
				firstDigit = (firstDigit + 1) % 3;
				newVal = (firstDigit * 10) + lastDigit;	// Get new hour
				LCDModule::alarmTime->setHour(newVal);
				break;
			case 1:	// Increment hour second digit ( 0 - 9 if first digit is 0 or 1 ; 0 - 4 if first digit is 2 )
				firstDigit = LCDModule::alarmTime->hour() / 10;
				lastDigit = LCDModule::alarmTime->hour() % 10;
				if (firstDigit == 0 || firstDigit == 1) {
					lastDigit = (lastDigit + 1) % 10;
				}
				else {	// Digit 2
					lastDigit = (lastDigit + 1) % 5;
				}
				newVal = (firstDigit * 10) + lastDigit; // Get new hour
				LCDModule::alarmTime->setHour(newVal);
				break;
			case 2: // Increment minutes first digit (0,1,2,3,4,5).
				firstDigit = LCDModule::alarmTime->minute() / 10;
				lastDigit = LCDModule::alarmTime->minute() % 10;
				firstDigit = (firstDigit + 1) % 6;
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setMinute(newVal);
				break;
			case 3:	// Increment minutes second digit (0,1,2,3,4,5,6,7,8,9)
				firstDigit = LCDModule::alarmTime->minute() / 10;
				lastDigit = LCDModule::alarmTime->minute() % 10;
				lastDigit = (lastDigit + 1) % 10;
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setMinute(newVal);
				break;
			case 4:	// Increment date first digit (0,1,2,3)
				firstDigit = LCDModule::alarmTime->day() / 10;
				lastDigit = LCDModule::alarmTime->day() % 10;
				firstDigit = (firstDigit + 1) % 4;
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setDay(newVal);
				break;
			case 5:	// Increment date last digit (0,1,2,3,4,5,6,7,8,9)
				firstDigit = LCDModule::alarmTime->day() / 10;
				lastDigit = LCDModule::alarmTime->day() % 10;
				lastDigit = (lastDigit + 1) % 10;
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setDay(newVal);
				break;
			case 6:	// Increment month first digit (0,1)
				firstDigit = LCDModule::alarmTime->month() / 10;
				lastDigit = LCDModule::alarmTime->month() % 10;
				firstDigit = (firstDigit + 1) % 2;
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setMonth(newVal);
				break;
			case 7:	// Increment month last digit (0 - 9 if first digit is 0, 0 - 2 if last digit is 1 )
				firstDigit = LCDModule::alarmTime->month() / 10;
				lastDigit = LCDModule::alarmTime->month() % 10;
				if (firstDigit == 0) {
					lastDigit = (lastDigit + 1) % 10;
				}
				else if (firstDigit == 1) {
					lastDigit = (lastDigit + 1) % 3;
				}
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setMonth(newVal);
				break;
			case 8:	// Increment year (offset from 2000) first digit (0,1,2,3,4,5,6,7,8,9)
				firstDigit = (LCDModule::alarmTime->year() - 2000) / 10;
				lastDigit = (LCDModule::alarmTime->year() - 2000) % 10;
				firstDigit = (firstDigit + 1) % 10;
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setYear(2000 + newVal);
				break;
			case 9:	// Increment year (offset from 2000) last digit (0,1,2,3,4,5,6,7,8,9)
				firstDigit = (LCDModule::alarmTime->year() - 2000) / 10;
				lastDigit = (LCDModule::alarmTime->year() - 2000) % 10;
				lastDigit = (lastDigit + 1) % 10;
				newVal = (firstDigit * 10) + lastDigit;
				LCDModule::alarmTime->setYear(2000 + newVal);
				break;
		}
		LCDModule::displayAlarmTime();
		LCDModule::gotoCursorPosition();
	}

	// Reset values
	alarmMenuNav->nextChar = false;
	alarmMenuNav->nextDigit = false;
}