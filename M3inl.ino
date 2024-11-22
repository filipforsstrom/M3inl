#include "InputDebounce.h"
#include "RTC.h"

#define BUTTON_DEBOUNCE_DELAY 20 // in ms

static const int pinLED = LED_BUILTIN; // 13
static const int pinButton = 2;
static const int pinSensor = A0; // sensor
static InputDebounce buttonA;
bool activeSensor = true;

HardwareSerial &serial = Serial1; // or Serial

void buttonTest_pressedCallback(uint8_t pinIn)
{
	RTCTime currenttime;
	RTC.getTime(currenttime);

	activeSensor = !activeSensor;
	if (activeSensor)
	{
		serial.print(currenttime.toString());
		serial.println(": Started reading sensor");
		digitalWrite(LED_BUILTIN, LOW);
	}
	else
	{
		serial.print(currenttime.toString());
		serial.println(": Stopped reading sensor");
		digitalWrite(LED_BUILTIN, HIGH);
	}
}

void setup()
{
	pinMode(pinLED, OUTPUT);
	serial.begin(115200);

	buttonA.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
	buttonA.setup(pinButton, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);

	RTC.begin();

	// A fallback time object, for setting the time if there is no time to retrieve from the RTC.
	RTCTime mytime(6, Month::NOVEMBER, 2023, 18, 12, 00, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_ACTIVE);

	// Tries to retrieve time
	RTCTime savedTime;

	RTC.getTime(savedTime);

	if (!RTC.isRunning())
	{
		// this means the RTC is waking up "as new"
		if (savedTime.getYear() == 2000)
		{
			RTC.setTime(mytime);
			serial.println("RTC was not running, setting time to default");
		}
		else
		{
			RTC.setTime(savedTime);
			serial.println("RTC was running, setting time to saved time");
		}
	}
}

void loop()
{
	static unsigned long lastSensorRead = 0;
	unsigned long now = millis();

	buttonA.process(now);

	if (activeSensor && (now - lastSensorRead >= 200))
	{
		int sensorValue = analogRead(pinSensor);
		serial.println(sensorValue);
		lastSensorRead = now;
	}
}