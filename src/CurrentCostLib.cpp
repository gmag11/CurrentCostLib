// 
// 
// 

#include "CurrentCostLib.h"

#define DBG_OUTPUT_PORT Serial

#ifdef DEBUG
#define DEBUGLOG(...) DBG_OUTPUT_PORT.printf(__VA_ARGS__)
#else
#define DEBUGLOG(...)
#endif

CurrentCost currentCost;

CurrentCost::CurrentCost() {
	DEBUGLOG("CurrentCost instance\n");
}

void CurrentCost::begin(Stream &port) {
#ifdef TEST
	sendTestMessage_ticker.attach(6, &CurrentCost::s_sendTestMessage, static_cast<void*>(this)); // Program send test message
#endif //TEST
	DEBUGLOG("\nCC Parser test started\n");
}

uint8_t CurrentCost::addSensor(Sensor_t sensor)
{
	if (sensors.size() < MAX_SENSORS) {
		sensors.push_back(sensor);
		return 0;
	}
	else {
		return -1;
	}
}

#ifdef TEST
void CurrentCost::process_ccost_xml_test(String msg) {

	int sensor_id = random(0, MAX_SENSORS);
	DEBUGLOG("Test message start: id = %d. Current sensor vector size %d\n", sensor_id, sensors.size());

	if (sensors.size() < sensor_id + 1) {
		std::size_t currentSize = sensors.size();
		std::size_t newSize = sensor_id + 1;
		
		for (int i = currentSize; i < newSize; i++) {
			sensors.push_back(Sensor_t(i));
			DEBUGLOG("------ Inserted new sensor id %d: new size %d\n", i, sensors.size());
		}
	}

	//sensors[sensor_id].sensor_id = sensor_id;
	sensors[sensor_id].setWatts(random(100, 1000));
	tempr = float(random(1000, 3000))/100;
	//sensors[sensor_id].time_sensor = millis();
	//sensors[sensor_id].diff = sensor[sensor_id].time_sensor - sensor[sensor_id].last_time_sensor;
	//sensors[sensor_id].last_time_sensor = sensor[sensor_id].time_sensor;

	//double sum = (double)((double)sensor[sensor_id].watts / 1000 * (double)sensor[sensor_id].diff / (1000 * 3600));

	//sensor[sensor_id].kwh += sum;
	//sensor[sensor_id].kwh_hour += sum;
	//sensor[sensor_id].kwh_day += sum;
	//sensor[sensor_id].kwh_month += sum;
	//sensor[sensor_id].kwh_year += sum;
	//sensor[sensor_id].valid = true;
	last_read_sensor = sensor_id;

	if (onSensorMeasEvent) {
		onSensorMeasEvent(sensor_id);
	}
}

void CurrentCost::s_sendTestMessage(void* arg) {
	CurrentCost* self = reinterpret_cast<CurrentCost*>(arg);
	self->sendTestMessage();
}

void CurrentCost::sendTestMessage() {
	process_ccost_xml_test("");
	DEBUGLOG("Read from sensor -> %s\n\n",get_sensor_data_str(last_read_sensor).c_str());
}

#endif //TEST

String CurrentCost::get_sensor_data_str(uint8_t id) {
	String temp = "";
	if (id >= 0 && id < MAX_SENSORS) {

		//temp += digitalClockString();
		//temp += " ";
		//temp += String(sensors[id].kwh, 6);//Delta time: %d
		//temp += " kWh. for ";
		//temp += String((double)((double)sensors[id].diff / 1000));
		//temp += " Sec. ";
		temp += "Sensor: ";
		temp += sensors[id].getSensorID();
		temp += " --> ";
		temp += sensors[id].getWatts();
		temp += " W. Temperature: ";
		temp += tempr;
	}
	//DEBUGLOG("Read from sensor -> %s\n", temp.c_str());
	return temp;
}

void CurrentCost::process_ccost_xml(String msg) {
	if (msg.indexOf("<hist>") == -1) { // If history message, discard it
		int idx = msg.indexOf("<sensor>"); // Look for sensor tag
		int sensor_id = (uint8_t)(msg.substring(idx + 8, idx + 9).toInt()); // Extract sensor id
																			//Serial.println(sensor_id);
		if ((sensor_id >= 0) || (sensor_id < MAX_SENSORS)) {
			//sensors[sensor_id].sensor_id = sensor_id;
			if (sensors.size() < sensor_id + 1) {
				std::size_t currentSize = sensors.size();
				std::size_t newSize = sensor_id + 1;

				for (int i = currentSize; i < newSize; i++) {
					sensors.push_back(Sensor_t(i));
					DEBUGLOG("------ Inserted new sensor id %d: new size %d\n", i, sensors.size());
				}
			}
			idx = msg.indexOf("<watts>"); // look for watts tag
			sensors[sensor_id].setWatts(msg.substring(idx + 7, idx + 12).toInt());
			idx = msg.indexOf("<tmpr>"); // Look for tmpr tag
			tempr = msg.substring(idx + 6, idx + 10).toFloat();

			// TODO time_sensor is not needed. It is used temporally only.
			//sensor[sensor_id].time_sensor = millis(); // Assign current time
			//sensor[sensor_id].diff = sensor[sensor_id].time_sensor - sensor[sensor_id].last_time_sensor; // Calculate last measurement age
			//sensor[sensor_id].last_time_sensor = sensor[sensor_id].time_sensor;
			//double sum = (double)((double)sensor[sensor_id].watts / 1000 * (double)sensor[sensor_id].diff / (1000 * 3600)); //Calculate kWh in this period using diff as time extension
																															//TODO If last sensor measure is not valid it should not add kWh.
			//sensor[sensor_id].kwh += sum;
			//sensor[sensor_id].kwh_hour += sum;
			//sensor[sensor_id].kwh_day += sum;
			//sensor[sensor_id].kwh_month += sum;
			//sensor[sensor_id].kwh_year += sum;
			sensors[sensor_id].setConnected(true);
			last_read_sensor = sensor_id;

			if (onSensorMeasEvent) {
				onSensorMeasEvent(sensor_id);
			}

		}
	}
}

void CurrentCost::onSensorEvent(onSensorEvent_t handler) {
	onSensorMeasEvent = handler;
}
