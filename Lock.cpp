#include "Lock.h"





Lock::Lock(PinConection conn, PinConection connLed, bool _isLock) :connPin(conn), _isLock(_isLock), lockLed(connLed) {
	pinMode(conn, OUT);
	digitalWrite(conn, OFF);
	Logger::info("Create Lock");
	Logger::debug("Lock pin "); Serial.print(conn);
}

const bool Lock::isLock() { Logger::debug("door is lock?");Serial.print(_isLock); return _isLock; }

const void Lock::open() {
	if (!isLock()) {
		digitalWrite(connPin, ON);
		delay(200);
		digitalWrite(connPin, OFF);
		Logger::info("Door is open");
		lockLed.flashTimes(2, 300);
	}

}

void Lock::lock() {
	this->_isLock = true;
	lockLed.turnOFF();
	Logger::info("Door is lock");

}

void Lock::unlock() {
	this->_isLock = false;
	lockLed.turnON();
	Serial.println("Door is unlock");
}

Lock::~Lock()
{
}
