#pragma once
#include "FingerPrintProxy.h"
#include "Lock.h"
#include "myLogger.h"
#include "RestClient.h"
#include "ArduinoJson.h"
#include "WifiConnectManager.h"



class updateManager
{
private:
	static RestClient *restClient;
	static FingerPrintProxy* fingerprint;
	static Lock* lock;
	static char* serverURL;
	static char * mac;
	static char * path;
	static unsigned long nextUpdate;
	static bool _init;
	static void handleResponse(String json);
public:
	static void init(FingerPrintProxy* fingerprint, Lock* lock,const char* serverURL, char * targetPath, int port = 0, char* sh1Fingerpeint = NULL);
	static void checkifActionNeeded();
};

