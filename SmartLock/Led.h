#pragma once
#include "NodeMcuConstants.h"
class Led
{
private:
	PinStatus status;
	PinConection connPin;
public:
	Led(PinConection conn):connPin(conn),status(OFF) {
		pinMode(conn, OUT);
		digitalWrite(conn, OFF);
	};
	PinStatus getStatus() { return status; };
	void turnON() {
		if (status == OFF) {
			digitalWrite(connPin, ON);
			status = ON;
		}
	};
	void turnOFF() {
		if (status == ON) {
			digitalWrite(connPin, OFF);
			status = OFF;
		}
	};
	~Led();
};

