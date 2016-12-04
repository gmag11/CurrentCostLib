#include <SoftwareSerial.h> // https://github.com/plerup/espsoftwareserial
#include <Ticker.h>
#include "CurrentCostLib.h"

#define DEBUG

#ifdef DEBUG
#define DEBUGLOG(...) DBG_OUTPUT_PORT.printf(__VA_ARGS__)
#else
#define DEBUGLOG(...)
#endif

SoftwareSerial sserial(D4, SW_SERIAL_UNUSED_PIN);

void processSensorEvent(uint8_t id, int watts, float tempr);

void setup()
{
	Serial.begin(115200);
	sserial.begin(57600);
	currentCost.begin(sserial);
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