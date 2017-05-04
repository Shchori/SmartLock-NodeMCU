#pragma once
#include "NodeMcuConstants.h"
#include "Led.h"
#include "myLogger.h"
class Lock
{
private:
	bool _isLock;
	const PinConection connPin;
	Led lockLed;
public:
	Lock(PinConection conn, PinConection connLed, bool _isLock = true);
	const bool isLock();;
	const void open();
	void lock();
	void unlock();
	~Lock();
};

