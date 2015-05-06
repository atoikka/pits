#include "stratoschem.h"

#include "misc.h" // Config

#include <wiringPi.h> // pinMode, digitalWrite
#include <stdbool.h> // bool
#include <stdio.h> // printf

bool relay_on = false;

void StratosChem_Setup()
{
	pinMode(Config.servo_pin, OUTPUT);
	digitalWrite(Config.servo_pin, HIGH);
}

void StratosChem_OnAltitudeUpdate(int altitude)
{
	// The GPS data on altitude is meters above mean sea level
	if (altitude >= Config.servo_height && !relay_on)
	{
		printf("StratosChem: Setting servo_pin LOW (relay is low-activated)\n");
		digitalWrite(Config.servo_pin, LOW);
		relay_on = true;
	}
}

// StratosChem_Tick
// Called real frequently from the main thread to help with simulation of reaching the height determined in variable servo_height,
// after which a HIGH signal is set to the servo_pin gpio pin.
void StratosChem_Tick()
{
	static current_runfor = 0;
	if(!relay_on)
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