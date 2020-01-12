#include "LCD_Helper.h"

LCDModule::LCDModule() {

}

void LCDModule::start() {
	LCDModule::liquidCrystal = new LiquidCrystal_I2C(LCDModule::address, 16, 2);
	LCDModule::liquidCrystal->init();
	LCDModule::liquidCrystal->backlight();
}

void LCDModule::clearScreen() {
	LCDModule::liquidCrystal->clear();
}

void LCDModule::gotoCursorPosition(uint8_t column, uint8_t row) {
	LCDModule::liquidCrystal->setCursor(column, row);
}

void LCDModule::noBlink() {
	LCDModule::liquidCrystal->noBlink();
}

void LCDModule::blinkOn() {
	LCDModule::liquidCrystal->blink_on();
}

void LCDModule::setCursor(uint8_t column, uint8_t row) {
	LCDModule::liquidCrystal->setCursor(column, row);
}

void LCDModule::print(const __FlashStringHelper* c) {
	LCDModule::liquidCrystal->print(c);
}

void LCDModule::displayAlarmIcon(bool status, uint8_t column, uint8_t row) {
	if (status) {
		LCDModule::liquidCrystal->createChar(0, ALARM_ICON);
		LCDModule::liquidCrystal->setCursor(column, row);
		LCDModule::liquidCrystal->write(0);
	}
	else {
		LCDModule::liquidCrystal->setCursor(column, row);
		LCDModule::liquidCrystal->print(" ");
	}
}

void LCDModule::print(const char* c) {
	LCDModule::liquidCrystal->print(c);
}

void LCDModule::createChar(uint8_t index, const char* charmap) {
	LCDModule::liquidCrystal->createChar(index, charmap);
}

void LCDModule::write(uint8_t index) {
	LCDModule::liquidCrystal->write(index);
}
