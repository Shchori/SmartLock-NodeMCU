#pragma once
#include "buttonAction.h"
#include "FingerPrintProxy.h"
#include "Lock.h"


class OutDoorButton :
	public ButtonAction
{
private:
	Lock* lock;
	FingerPrintProxy* fingerprint;
	
public:

	OutDoorButton(Lock * lock, FingerPrintProxy *fingerprint) :lock(lock), fingerprint(fingerprint) {};
	~OutDoorButton() {};
	void onLongPress();
	void onShortPress();
};

