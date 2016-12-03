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
#include <list>

using namespace std;

#define MAX_SENSORS 10		//Max number of channels of Current Cost sensor

class Sensor_t {
private:
	
	uint8_t sensor_id;
	//long time_sensor;
	//long last_time_sensor;
	//int diff;
	int watts;
	//float tempr;
	//double kwh;
	//double kwh_hour;
	//double kwh_day;
	//double kwh_month;
	//double kwh_year;
	boolean connected = false;

public:

	Sensor_t(uint8_t id);

	int getWatts();
	int getSensorID();
	boolean isConnected();

};

class CurrentCost {

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
	String get_sensor_data_str();

	/**
	* Constructs CurrentCost object and initiates periodic tasks
	* @param[out] CurrentCost object
	*/
	CurrentCost();

	uint8_t addSensor(sensor_t sensor);

private:

	uint8_t last_read_sensor = -1; // Sensor read in last lecture
	std::list<sensor_t> sensors; // Sensor data storage

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
	static void sendTestMessage();
	/**
	* Generates a string with current time and date
	* @param[out] String with current time and date
	*/
	String digitalClockString();

	/**
	* Accesory function to correctly format time data
	* @param[in] a digit.
	* @param[out] Digit with leading zero if needed
	*/
	String fillDigits(int digits);


#endif //TEST
};

#endif //_CURRENTCOSTLIB

