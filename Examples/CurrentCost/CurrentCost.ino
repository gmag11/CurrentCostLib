#include <ArduinoJson.h>
#include <Ticker.h>
#include "CurrentCostLib.h"

#define DEBUG

#ifdef DEBUG
#define DEBUGLOG(...) DBG_OUTPUT_PORT.printf(__VA_ARGS__)
#else
#define DEBUGLOG(...)
#endif

void processSensorEvent(uint8_t id, int watts, float tempr);

void setup()
{
	Serial.begin(115200);
	currentCost.begin(Serial);
	currentCost.onSensorEvent(processSensorEvent);
}

void loop()
{
	currentCost.handle();
}

void processSensorEvent(uint8_t id, int watts, float tempr) {
	DEBUGLOG("---->  Event %d\n", id);
	DEBUGLOG("Power: %d W\n", watts);
	DEBUGLOG("Temp: %.2f C\n\n", tempr);
}