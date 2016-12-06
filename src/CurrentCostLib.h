// CurrentCostLib.h

#ifndef _CURRENTCOSTLIB
#define _CURRENTCOSTLIB

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ticker.h>
#include <vector>
#include <functional>
#include <Stream.h>
#include "Sensor.h"

using namespace std;

#define MAX_SENSORS 2		//Max number of channels of Current Cost sensor
#define TEST // Uncomment to simulate readings from CurrentCost sensor
#ifdef TEST
#define TEST_PERIOD 1
#endif

typedef std::function<void(uint8_t,int,float)> onSensorEvent_t;

class CurrentCost {
	friend class Sensor_t;

public:

	/**
	* Gets sensor data to a String for output via Serial
	* @param[out] Prettified data
	* @param[in] Sensor ID
	*/
	String getSensorDataStr(uint8_t id);

	/**
	* Constructs CurrentCost object and initiates periodic tasks
	* @param[out] CurrentCost object
	*/
	CurrentCost();

	/**
	* Starts power monitor
	* @param[in] Stream to read for XML data
	*/
	void begin(Stream &port);

	/**
	* Process input stream to search for data
	*/
	void handle();

	/**
	* Set a callback that triggers after a sync trial.
	* @param[in] function with void(NTPSyncEvent_t) or std::function<void(NTPSyncEvent_t)> (only for ESP8266)
	*				NTPSyncEvent_t equals 0 is there is no error
	*/
	void onSensorEvent(onSensorEvent_t handler);

private:

	uint8_t last_read_sensor = -1; // Sensor read in last lecture
	uint8_t numSensors = 0; // Number of sensors in memory
	std::vector <Sensor_t> sensors; // Sensor data storage
	float tempr; // Room temperature
	Stream *port; // Stream to read from
	onSensorEvent_t onSensorMeasEvent; // Function to be triggered when a measurement is received

	/**
	* Processes CurrentCost XML message, extracting measurements and storing them on sensors
	* @param[in] XML String got from CurrentCost serial port
	*/
	void process_ccost_xml(String msg);

	/**
	* Reserve memory for a sensor whose data has been received
	* @param[in] new sensor to add to memory
	* @param[out] Status code 0 = OK, -1 = error
	*/
	uint8_t addSensor(Sensor_t sensor);

	/**
	* Gets last measured temperature
	* @param[out] Room temperature
	*/
	float getTemp() {
		return tempr;
	}

	/**
	* Gets last measured power for given sensor
	* @param[out] Watts
	* @param[in] Sensor ID
	*/
	int getWatts(uint8_t id);


#ifdef TEST
	Ticker sendTestMessage_ticker; // Ticker to simulate measurement result

	/**
	* Simulate CurrentCost message every n seconds and show data over Serial
	*/
	void sendTestMessage();
	static void s_sendTestMessage(void* arg);

#endif //TEST
};

extern CurrentCost currentCost;

#endif //_CURRENTCOSTLIB

