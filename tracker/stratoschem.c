#include "stratoschem.h"

#include "misc.h" // Config

#include <wiringPi.h> // pinMode, digitalWrite

bool servo_high = false;

void StratosChem_Setup()
{
	pinMode(Config.servo_pin, OUTPUT);
}

void StratosChem_OnAltitudeUpdate(int altitude)
{
	// The GPS data on altitude is meters above mean sea level
	if (altitude >= Config.servo_height && !servo_high)
	{
		digitalWrite(Config.servo_pin, HIGH);
		servo_high = true;
	}
}