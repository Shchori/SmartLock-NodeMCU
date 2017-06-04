#pragma once

#define PORTAL_TIME_OUT 300

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "Led.h"
#include "NodeMcuConstants.h"
#include "myLogger.h"


class WifiConnectManager
{
private:
	static bool connected;
	static bool _init;
	static Led* disconnectedLed;
public:
	static void init(PinConection led);
	static bool checkConnection();
	static char* getMacAdress();
	static void resetSetting();
};

