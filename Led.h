#pragma once
#include "NodeMcuConstants.h"
#include "myLogger.h"
class Led 
{
private:
	PinStatus status;
	const PinConection connPin;
public:
	Led(PinConection conn);
	const PinStatus getStatus();
	void turnON();
	void turnOFF();
	const void flashTimes(int times, int delayMillis);
	~Led();
};

