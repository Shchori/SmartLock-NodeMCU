#include "OutDoorButton.h"

void OutDoorButton::onLongPress() {
	if (lock->isLock()) {
		int id = fingerprint->getFingerprintID();
		if (id > -1) {
			lock->unlock();
			lock->open();
			updateManager::updateServerLocalAction("unlock", id);
		}
	}
	else {
		lock->lock();
		updateManager::updateServerLocalAction("lock");
	}
}

void OutDoorButton::onShortPress() {
	if (!lock->isLock()) {
		lock->open();
	}
}
