#include <Arduino.h>

enum Commands
{
	ON,
	OFF,
	UNKNOWN
};

const char* commandToString(Commands command)
{
	switch (command)
	{
	case ON:
		return "ON";
	case OFF:
		return "OFF";
	default:
		return "UNKNOWN";
	}
}

Commands getCommandFromString(String message)
{
	if (message == "ON")
	{
		return ON;
	}
	else if (message == "OFF")
	{
		return OFF;
	}
	else
	{
		return UNKNOWN;
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Hello World");
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	if (Serial.available() > 0)
	{
		String message = Serial.readStringUntil('\n');
		message.trim();
		Commands command = getCommandFromString(message);
		Serial.print("Command: ");
		Serial.println(commandToString(command));
		switch (command)
		{
		case ON:
			digitalWrite(LED_BUILTIN, HIGH);
			break;
		case OFF:
			digitalWrite(LED_BUILTIN, LOW);
			break;
		default:
			break;
		}
	}
}
