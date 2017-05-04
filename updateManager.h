#pragma once
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "myLogger.h"
#include "FingerPrintProxy.h"
#include "Lock.h"
#include "myLogger.h"
#include "RestClient.h"
#include "ArduinoJson.h"



class updateManager
{
private:
	static RestClient *restClient;
	static bool connected;
	static bool _init;
	static void checkConnection();
	static FingerPrintProxy* fingerprint;
	static Lock* lock;
	static char* serverURL;
	static const char * mac;
public:
	static void init(FingerPrintProxy* fingerprint, Lock* lock,const char* serverURL,int port);
	static void checkifActionNeeded();
};

