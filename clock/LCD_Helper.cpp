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

	char buffer[10];
	// HH:MM of alarm time
	LCDModule::lcdModule->setCursor(5, 0);
	sprintf(buffer, "%02d:%02d", dateTime->hour(), dateTime->minute());
	LCDModule::lcdModule->print(buffer);
	// DD/MM/YYYY of alarm time
	LCDModule::lcdModule->setCursor(5, 1);
	sprintf(buffer, "%02d/%02d/%d", dateTime->day(), dateTime->month(), dateTime->year());
	LCDModule::lcdModule->print(buffer);

	LCDModule::lcdModule->setCursor(5, 0);
	LCDModule::lcdModule->blink_on();
}

void LCDModule::adjustAlarmTiming(AlarmMenuNavigation* alarmMenuNav) {
	if (alarmMenuNav->nextChar) {
		// Serial.println(F("Moving to next char"));
		LCDModule::cursorPosition += 1; // Increment
		LCDModule::cursorPosition = LCDModule::cursorPosition % 10;
		const uint8_t* arr = CURSOR_POSITION[LCDModule::cursorPosition];
		LCDModule::lcdModule->setCursor(arr[0], arr[1]);
	}

	if (alarmMenuNav->nextDigit) {
		
	}

	// Reset values
	alarmMenuNav->nextChar = false;
	alarmMenuNav->nextDigit = false;
}