#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY 20 // in ms

static const int pinLED = LED_BUILTIN; // 13
static const int pinButton = 2;
static const int pinSensor = A0; // sensor
static InputDebounce buttonA;
bool activeSensor = true;

void buttonTest_pressedCallback(uint8_t pinIn)
{
	activeSensor = !activeSensor;
	if (activeSensor)
	{
		digitalWrite(LED_BUILTIN, LOW);
	}
	else
	{
		digitalWrite(LED_BUILTIN, HIGH);
	}
}

void buttonTest_releasedCallback(uint8_t pinIn)
{
}

void buttonTest_pressedDurationCallback(uint8_t pinIn, unsigned long duration)
{
}

void buttonTest_releasedDurationCallback(uint8_t pinIn, unsigned long duration)
{
}

void setup()
{
	pinMode(pinLED, OUTPUT);
	Serial.begin(115200);
	Serial1.begin(115200);

	buttonA.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
	buttonA.setup(pinButton, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);

	// examples
	// buttonA.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, buttonTest_releasedDurationCallback); // no continuous pressed-on time duration, ...
	// buttonA.setup(pinButton);
	// buttonA.setup(pinButton, BUTTON_DEBOUNCE_DELAY);
	// buttonA.setup(pinButton, DEFAULT_INPUT_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_UP_RES);
	// buttonA.setup(pinButton, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES, 0, InputDebounce::ST_NORMALLY_CLOSED); // switch-type normally closed
}

void loop()
{
	unsigned long now = millis();

	buttonA.process(now);

	if (activeSensor)
	{
		int sensorValue = analogRead(pinSensor);
		Serial.println(sensorValue);
		Serial1.println(sensorValue);
	}

	delay(1);
}