#include "stratoschem.h"

#include "misc.h" // Config

#include <wiringPi.h> // pinMode, digitalWrite
#include <stdbool.h> // bool
#include <stdio.h> // printf

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
		printf("StratosChem: Setting servo_pin HIGH\n");
		digitalWrite(Config.servo_pin, HIGH);
		servo_high = true;
	}
}

// StratosChem_Tick
// Called real frequently from the main thread to help with simulation of reaching the height determined in variable servo_height,
// after which a HIGH signal is set to the servo_pin gpio pin.
void StratosChem_Tick()
{
	static current_runfor = 0;
	if(!servo_high)
	{
		int runningfor = time(0) - Config.startup_unix;
		if(runningfor != current_runfor) {
			printf("TEST: running for %d sec...\n", runningfor);
		}
		if(runningfor >= 15)
		{
			printf("TEST: execution simulation...\n");
			StratosChem_OnAltitudeUpdate(Config.servo_height);
		}
	}
}