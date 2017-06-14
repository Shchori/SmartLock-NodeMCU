#pragma once
#include "WifiConnectManager.h"
#include "Lock.h"


class InDoorButton:
	public ButtonAction
{
private:
	Lock* lock;
public:
	InDoorButton(Lock* lock):lock(lock){};
	~InDoorButton() {};
	void onLongPress() {
		WifiConnectManager::resetSetting();
	};
	void onShortPress() {
		if (lock->isLock()) {
				lock->unlock();
				updateManager::updateServerLocalAction("unlock");
		}else {
			lock->lock();
			updateManager::updateServerLocalAction("lock");
		}
	};
};