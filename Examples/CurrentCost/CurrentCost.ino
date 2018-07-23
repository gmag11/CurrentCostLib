#include <SoftwareSerial.h> // https://github.com/plerup/espsoftwareserial
#include <CurrentCostLib.h>

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
	Serial.printf ("---->  Event %d\n", id);
	Serial.printf ("Power: %d W\n", watts);
	Serial.printf ("Temp: %.2f C\n\n", tempr);
}