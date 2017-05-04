#pragma once
#include "buttonAction.h"
#include "FingerPrintProxy.h"
#include "Lock.h"


class LockButton :
	public ButtonAction
{
private:
	Lock* lock;
	FingerPrintProxy* fingerprint;
	
public:

	LockButton(Lock * lock, FingerPrintProxy *fingerprint) :lock(lock), fingerprint(fingerprint) {};
	~LockButton() {};
	void onLongPress();
	void onShortPress();
};

