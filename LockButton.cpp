#include "LockButton.h"

void LockButton::onLongPress() {
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

void LockButton::onShortPress() {
	if (!lock->isLock()) {
		lock->open();
	}
}
