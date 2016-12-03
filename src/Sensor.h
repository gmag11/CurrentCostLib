// Sensor.h

#ifndef _SENSOR_h
#define _SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Sensor_t {
protected:

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

	int getWatts() {
		return watts;
	}

	void setWatts(int value) {
		watts = value;
	}

	uint8_t getSensorID() {
		return sensor_id;
	}

	void setSensorID(uint8_t id) {
		sensor_id = id;
	}

	boolean isConnected() {
		return connected;
	}

	void setConnected(boolean status) {
		connected = status;
	}

};

#endif

