#include "Lock.h"





Lock::Lock(PinConection conn, PinConection lockLed, PinConection unLockLed, bool _isLock) :connPin(conn), _isLock(_isLock), lockLed(lockLed), unLockLed(unLockLed) {
	pinMode(conn, OUT);
	digitalWrite(conn, OFF);
	Logger::info("Create Lock");
	Logger::debug("Lock pin "); Serial.print(conn);
	if (_isLock) {
		lock();
	}
	else {
		unlock();
	}
}

const bool Lock::isLock() { Logger::debug("door is lock?");Serial.print(_isLock); return _isLock; }

const void Lock::open() {
	if (!isLock()) {
		digitalWrite(connPin, ON);
		delay(200);
		digitalWrite(connPin, OFF);
		Logger::info("Door is open");
	}
	lockLed.flashTimes(1, 300);
}

void Lock::lock() {
	this->_isLock = true;
	unLockLed.turnOFF();
	lockLed.turnON();
	Logger::info("Door is lock");

}

void Lock::unlock() {
	this->_isLock = false;
	unLockLed.turnON();
	lockLed.turnOFF();
	Logger::info("Door is unlock");
}

Lock::~Lock()
{
}
