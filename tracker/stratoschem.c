#include "stratoschem.h"

#include "misc.h" // Config

#include <wiringPi.h> // pinMode, digitalWrite
#include <stdbool.h> // bool
#include <stdio.h> // printf
#include <time.h> // time

bool chem_done = false;
bool relay_on = false;
int relay_on_since = -1;
int max_height = -1000;

void StratosChem_Setup()
{
	pinMode(Config.servo_pin, OUTPUT);
	digitalWrite(Config.servo_pin, HIGH);
}

void StratosChem_OnAltitudeUpdate(int altitude)
{
	// The GPS data on altitude is meters above mean sea level
	if(!chem_done)
	{
		if(altitude >= max_height) {
			max_height = altitude;
		}
		else if(altitude < max_height)
		{
			int diff = max_height - altitude;
			if(diff >= 200)
			{
				// Probably falling from the sky before experiment is done, so execute the experiment right away
				StratosChem_RunExperiment();
			}
		}
		
		if (altitude >= Config.servo_height)
		{
			// At the right height, execute the experiment
			StratosChem_RunExperiment();
		}
	}
}

// StratosChem_RunExperiment
void StratosChem_RunExperiment()
{
	printf("StratosChem: Executing keemiakatse!");
	digitalWrite(Config.servo_pin, LOW); // Relay is low-activated, so it must be set to LOW
	relay_on = true;
	relay_on_since = (int)time(0);
	chem_done = true;
}

// StratosChem_Tick
// Called real frequently from the main thread to help with simulation of reaching the height determined in variable servo_height,
// after which a HIGH signal is set to the servo_pin gpio pin.
void StratosChem_Tick()
{
	static current_runfor = 0;
	if(chem_done == false)
	{
		if(relay_on == false)
		{
			if(Config.servo_test != 0) 
			{
				// If "servo_test" is set to 1 in the config, dp chemstry after 15 seconds of running
				int runningfor = time(0) - Config.startup_unix;
				if(runningfor != current_runfor) {
					printf("TEST: running for %d sec...\n", runningfor);
					current_runfor = runningfor;
				}
				
				if(runningfor >= 15) {
					StratosChem_RunExperiment();
				}
			}
		}
	}
	else if(chem_done == true)
	{
		if(relay_on == true)
		{
			// Keep the relay on for only 3 minutes after keemiakatse execution
			int relay_on_for = time(0) - relay_on_since;
			if(relay_on_for >= (3*60)) {
				digitalWrite(Config.servo_pin, HIGH);
				relay_on = false;
			}
		}
	}
}