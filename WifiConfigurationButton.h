#pragma once
#include <WiFiManager.h>
#define PORTAL_TIME_OUT 300
#include "myLogger.h"
class WifiConfigurationButton:public ButtonAction
{
public:
	WifiConfigurationButton(){};
	~WifiConfigurationButton() {};
	void onLongPress() {
		Logger::info("reset setting and start portal to sec:");Serial.print(PORTAL_TIME_OUT);
		WiFiManager wifiManager;
		wifiManager.resetSettings();
		wifiManager.setConfigPortalTimeout(PORTAL_TIME_OUT);
		wifiManager.autoConnect("SmartLock","12345678");

	};
	void onShortPress() {};
};

