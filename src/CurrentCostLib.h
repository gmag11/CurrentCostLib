// CurrentCostLib.h

#ifndef _CURRENTCOSTLIB
#define _CURRENTCOSTLIB

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ticker.h>
#include <ArduinoJson.h>
#include <vector>
#include <functional>
#include <Stream.h>
#include "Sensor.h"

using namespace std;

#define MAX_SENSORS 2		//Max number of channels of Current Cost sensor
#define TEST
#define DEBUG

typedef std::function<void(uint8_t)> onSensorEvent_t;

class CurrentCost {
	friend class Sensor_t;

public:

#ifdef TEST
	/**
	* Simulates CurrenCost measurement result
	* @param[in] Dummy string to make this call compatible with process_ccost_xml(String)
	*/
	void process_ccost_xml_test(String msg);
#endif //TEST

	/**
	* Gets sensor data to a String for output via Serial
	* @param[out] Prettified data
	*/
	String get_sensor_data_str(uint8_t id);

	/**
	* Constructs CurrentCost object and initiates periodic tasks
	* @param[out] CurrentCost object
	*/
	CurrentCost();

	void begin(Stream &port);

	uint8_t addSensor(Sensor_t sensor);

	/**
	* Set a callback that triggers after a sync trial.
	* @param[in] function with void(NTPSyncEvent_t) or std::function<void(NTPSyncEvent_t)> (only for ESP8266)
	*				NTPSyncEvent_t equals 0 is there is no error
	*/
	void onSensorEvent(onSensorEvent_t handler);

private:

	uint8_t last_read_sensor = -1; // Sensor read in last lecture
	uint8_t numSensors = 0;
	std::vector <Sensor_t> sensors; // Sensor data storage
	float tempr;
	onSensorEvent_t onSensorMeasEvent;

	/**
	* Processes CurrentCost XML message, extracting measurements and storing them on sensors
	* @param[in] XML String got from CurrentCost serial port
	*/
	void process_ccost_xml(String msg);



#ifdef TEST
	Ticker sendTestMessage_ticker; // Ticker to simulate measurement result
#endif //TEST
#ifdef TEST

	/**
	* Simulate CurrentCost message every n seconds and show data over Serial
	*/
	void sendTestMessage();
	static void s_sendTestMessage(void* arg);

#endif //TEST
};

extern CurrentCost currentCost;

#endif //_CURRENTCOSTLIB

