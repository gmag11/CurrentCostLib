// 
// 
// 

#include "CurrentCostLib.h"

//#define DEBUG // Uncomment to send debug messages over serial

#ifdef DEBUG
#define DBG_OUTPUT_PORT Serial
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
	sendTestMessage_ticker.attach(TEST_PERIOD, &CurrentCost::s_sendTestMessage, static_cast<void*>(this)); // Program send test message
#endif //TEST
	this->port = &port;
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
void CurrentCost::process_ccost_xml(String msg) {

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
	sensors[sensor_id].setWatts(random(sensor_id*100, (sensor_id+1)*100));
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
		onSensorMeasEvent(sensor_id, getWatts(sensor_id), tempr);
	}
}

void CurrentCost::s_sendTestMessage(void* arg) {
	CurrentCost* self = reinterpret_cast<CurrentCost*>(arg);
	self->sendTestMessage();
}

void CurrentCost::sendTestMessage() {
	process_ccost_xml("");
	DEBUGLOG("Read from sensor -> %s\n\n",getSensorDataStr(last_read_sensor).c_str());
}

void CurrentCost::handle() {

}

#else //No TEST

void CurrentCost::process_ccost_xml(String msg) {
	DEBUGLOG ("%s\n\n", msg.c_str ());
	if (msg.indexOf("<hist>") == -1) { // If history message, discard it
		int idxSensor = msg.indexOf("<sensor>"); // Look for sensor tag
		int idxWatts = msg.indexOf("<watts>"); // look for watts tag
		int idxTemp = msg.indexOf("<tmpr>"); // Look for tmpr tag
		if (idxSensor >= 0 && idxWatts >= 0 && idxTemp >= 0) { // Discard message if not all data is inside
			int sensor_id = (uint8_t)(msg.substring(idxSensor + 8, idxSensor + 9).toInt()); // Extract sensor id
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
				//idx = msg.indexOf("<watts>"); // look for watts tag
				sensors[sensor_id].setWatts(msg.substring(idxWatts + 7, idxWatts + 12).toInt());
				//idx = msg.indexOf("<tmpr>"); // Look for tmpr tag
				tempr = msg.substring(idxTemp + 6, idxTemp + 10).toFloat();

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
					onSensorMeasEvent(sensor_id, getWatts(sensor_id), tempr);
				}

			}
		}
	}
}

void CurrentCost::handle() {
	if (port->available()) {
		String mensaje = port->readStringUntil('\n');
		DEBUGLOG("%s\n", mensaje.c_str());
		process_ccost_xml(mensaje);
	}
}

#endif //TEST

String CurrentCost::getSensorDataStr(uint8_t id) {
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

int CurrentCost::getWatts(uint8_t id)
{
	if (id >= 0 && id < sensors.size()) {
		return sensors[id].getWatts();
	}
	else {
		return -1;
	}
}


void CurrentCost::onSensorEvent(onSensorEvent_t handler) {
	DEBUGLOG("-- Handler registered\n");
	onSensorMeasEvent = handler;
}
