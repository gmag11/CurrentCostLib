//#include "Sensor.h"
#include <ArduinoJson.h>
#include <Ticker.h>
#include "CurrentCostLib.h"

void setup()
{
	Serial.begin(115200);
	currentCost.begin(Serial);
  /* add setup code here */

}

void loop()
{
	
  /* add main program code here */

}
