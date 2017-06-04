#include "Led.h"

Led::Led(PinConection conn) :connPin(conn), status(OFF) {
	Logger::info("Create led");
	Logger::debug("Led pin "); Serial.print(conn);
	digitalWrite(conn, OFF);
	pinMode(conn, OUT);
}

const PinStatus Led::getStatus() { return status; }
void Led::turnON() {
	if (status == OFF) {
		Logger::info("Turn on led");
		Logger::debug("Led pin :"); Serial.print(connPin);
		digitalWrite(connPin, ON);
		status = ON;
	}
}

void Led::turnOFF() {
	if (status == ON) {
		Logger::info("Turn off led");
		Logger::debug("Led pin :"); Serial.print(connPin);
		digitalWrite(connPin, OFF);
		status = OFF;
	}
}

const void Led::flashTimes(int times, int delayMillis) {
	bool tempStatus = this->status;
	Logger::info("flash led times:"); Serial.print(times);
	Logger::debug("Led pin :"); Serial.print(connPin);
	Logger::debug("delay :"); Serial.print(delayMillis);
	digitalWrite(connPin, OFF);
	for (int i = 0; i < times; ++i) {
		digitalWrite(connPin, ON);
		delay(delayMillis);
		digitalWrite(connPin, OFF);
	}
	if (tempStatus) {
		digitalWrite(connPin, ON);
	}
	else {
		digitalWrite(connPin, OFF);
	}
}
