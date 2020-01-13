# Arduino Clock

_Arduino clock with alarm, temperature & humidity_

## Features

1. Clock 
2. Alarm with buzzer
3. Temperature and Humidity reading

## Components

1. Arduino Uno
2. DS1307 RTC
3. DHT22 Temperature and Humidity Sensor
4. Digital Buzzer
5. Push Button (x3)
6. 16x2 LCD
7. LCD I2C interface module

<br>

![circuit](https://user-images.githubusercontent.com/39665412/72229989-91ed7100-35ed-11ea-9246-1fcbcc2185cb.png)

## Usage Instructions

The program has 2 menus - The main menu and the alarm menu. The main menu displays the current time and date, as well as temperature and humidity information. The alarm menu allows users to change the alarm timing. Changes to the alarm timing are automatically saved.

### Buttons

There are 3 push buttons. Below are the functionality of each button depending on the menu.

1. Button One (Pin 5)
    1. __Main Menu__ - Toggle to the next menu
    2. __Alarm Menu__ - Toggle to the next menu

2. Button Two (Pin 6)
    1. __Alarm Menu__ - Move LCD cursor to the next position

3. Button Three (Pin 7)
    1. __Main Menu__ - Set alarm ON/OFF
    2. __Alarm Menu__ - Increment digit

If the alarm is ringing, then pressing any button will turn off the alarm.

## Improvements

1. Contact bounce - The contact bounce of the push buttons lead to multiple clicks being registered for a single click. The problem is somewhat mitigated by delaying loop execution. Maybe push button hardware problem?

2. Validate alarm time - There is no error checking to check if user enters an invalid date for the alarm. Maybe submit a pull request on [this suggestion](https://github.com/adafruit/RTClib/issues/127) for DateTime library?

## License

Arduino-Clock is licensed under the GNU General Public License v3.0.