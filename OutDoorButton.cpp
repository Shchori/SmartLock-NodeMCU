#include "OutDoorButton.h"

void OutDoorButton::onLongPress() {
	if (lock->isLock()) {
		int id = fingerprint->getFingerprintID();
		if (id > -1) {
			lock->unlock();
			lock->open();
		}
	}
	else {
		lock->lock();
	}
}

void OutDoorButton::onShortPress() {
	if (!lock->isLock()) {
		lock->open();
	}
}
