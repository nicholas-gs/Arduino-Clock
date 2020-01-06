/*
 Name:		clock.ino
 Created:	1/3/2020 2:35:06 PM
 Author:	Nicho
*/

#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include "LCD_Helper.h"

const uint8_t MAIN_MENU = 0;
const uint8_t ALARM_MENU = 1;
const uint8_t NUMBER_OF_MENUS = 2;

// Pins
const uint8_t BUTTON_ONE_PIN = 5;
const uint8_t BUTTON_TWO_PIN = 6;
const uint8_t BUTTON_THREE_PIN = 7;
const uint8_t DHT_PIN = 2;

// DHT22 Temperature and Humidity Sensor Module
DHT_Unified dhtModule(DHT_PIN, DHT22);
// RTC Module
RTC_DS1307 rtcModule;
// LCD
LCDModule lcdModule;

// loop variables
const uint8_t loop_delay = 150; // Loop delay of 150ms
const uint16_t DHT_delay = 2000; // Update temperature & humidity data every 2 seconds
unsigned long nextTrigTime = 0;
unsigned long nextDHTTrigTime = 0;
static uint8_t menuChoice = MAIN_MENU;

// Button
bool buttonOneState = true;
bool buttonTwoState = true;
bool buttonThreeState = true;
// bool if alarm is ON or OFF
bool alarmOn = false;
AlarmMenuNavigation alarmMenuNav;

// RTC
static DateTime dateTime;
static DateTime nextTime;
// Alarm timing
static DateTime alarmTime;

// DHT22 Event
static sensors_event_t dht22Event;
static TempHumidityDataPoint dataPoint;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	
	pinMode(BUTTON_ONE_PIN, INPUT_PULLUP);
	pinMode(BUTTON_TWO_PIN, INPUT_PULLUP);
	pinMode(BUTTON_THREE_PIN, INPUT_PULLUP);

	rtcModule.begin();
	rtcModule.isrunning();

	dhtModule.begin();
	
	lcdModule.start();

	// Set the time for the RS1307 RTC only once!
	// dateTime = DateTime(F(__DATE__), F(__TIME__));
	// rtcModule.adjust(dateTime);

	// Initial alarmTime when arduino starts up
	alarmTime = rtcModule.now();

	delay(50);
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (millis() >= nextTrigTime) {
		readInButtonsState();
		nextTime = rtcModule.now();				// Get the current time from RTC
		nextTrigTime = millis() + loop_delay;
		
		switch (menuChoice) {
		case 0:
			displayMainMenu();
			break;
		case 1:
			displayAlarmMenu();
			break;
		}

	}
}

void displayMainMenu() {
	lcdModule.initMainMenu(&alarmOn);
	
	// If at least 1 second has passed from the current time shown in LCD module, then update LCD with current time
	if ((nextTime - dateTime).seconds() >= 1) {
		dateTime = nextTime;
		lcdModule.updateMainMenuTime(&dateTime);
	}

	// Update temperature and humidity 
	if (millis() >= nextDHTTrigTime) {
		nextDHTTrigTime = millis() + DHT_delay;
		dhtModule.temperature().getEvent(&dht22Event);
		if (!isnan(dht22Event.temperature)) {
			dataPoint.temperature = dht22Event.temperature;
		}

		dhtModule.humidity().getEvent(&dht22Event);
		if (!isnan(dht22Event.relative_humidity)) {
			dataPoint.relative_humidity = dht22Event.relative_humidity;
		}

		lcdModule.updateMainMenuTempHumidity(&dataPoint);
	}
}

void displayAlarmMenu() {
	nextTime = rtcModule.now();
	dateTime = nextTime;
	lcdModule.startAlarmMenuScreen(&alarmTime);

	// While the user has not pressed the menu selection button (button 1), will continue to be in the alarm menu
	while (menuChoice == ALARM_MENU) {
		if (millis() >= nextTrigTime) {
			nextTrigTime += loop_delay;
			readInButtonsState();
			lcdModule.adjustAlarmTiming(&alarmMenuNav);
		}
	}
}

/*
	Read in status of push buttons. Since input pins are tied to pullup resistors - 
	1. Logic 1 (True) if button not pressed
	2. Logic 0 (False) if button pressed
*/
void readInButtonsState() {
	buttonOneState = digitalRead(BUTTON_ONE_PIN);
	buttonTwoState = digitalRead(BUTTON_TWO_PIN);
	buttonThreeState = digitalRead(BUTTON_THREE_PIN);

	if (!buttonOneState) {
		Button_One_Pressed();
		buttonOneState = true;
	}

	if (!buttonTwoState) {
		Button_Two_Pressed();
		buttonTwoState = true;
	}

	if (!buttonThreeState) {
		Button_Three_Pressed();
		buttonThreeState = true;
	}
}

/*
	Main Menu -- Menu selection button
	Alarm Menu -- Menu selection button
*/
void Button_One_Pressed() {
	if (menuChoice == MAIN_MENU || menuChoice == ALARM_MENU) {
		menuChoice += 1;
		menuChoice = menuChoice % NUMBER_OF_MENUS;
	}
	lcdModule.clearScreen();
}

/*
	Alarm Menu -- Move to next character
*/
void Button_Two_Pressed() {
	if (menuChoice == ALARM_MENU) {
		alarmMenuNav.nextChar = true;
	}
}

/*
	Main Menu -- Toggle alarm ON/OFF
	Alarm Menu -- Increment to next digit
*/
void Button_Three_Pressed() {
	if (menuChoice == MAIN_MENU) {
		alarmOn = !alarmOn;
		lcdModule.toggleAlarmIcon(&alarmOn);
	}
	else if (menuChoice == ALARM_MENU) {
		alarmMenuNav.nextDigit = true;
	}
}

void triggerBuzzer() {

}
