#include "stratoschem.h"

#include "misc.h" // Config

#include <wiringPi.h> // pinMode, digitalWrite
#include <stdbool.h> // bool

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

// StratosChem_Tick
// Called real frequently from the main thread to help with simulation of reaching the height determined in variable servo_height,
// after which a HIGH signal is set to the servo_pin gpio pin.
void StratosChem_Tick()
{
	if(!servo_high)
	{
		int runningfor = time(0) - Config.startup_unix;
		if(runningfor >= 15)
		{
			StratosChem_OnAltitudeUpdate(Config.servo_height);
		}
	}
}