#pragma once
#include "FingerPrintProxy.h"
#include "Lock.h"
#include "myLogger.h"
#include "RestClient.h"
#include "ArduinoJson.h"
#include "WifiConnectManager.h"

#define UPDATE_TIME 10

class updateManager
{
private:
	static RestClient *restClient;
	static FingerPrintProxy* fingerprint;
	static Lock* lock;
	static int serverErrorTime;
	static char* serverURL;
	static char * mac;
	static char * path;
	static char * logPath;
	static unsigned long nextUpdate;
	static bool _init;
	static void handleResponse(String json);
public:
	static void init(FingerPrintProxy* fingerprint, Lock* lock,const char* serverURL, char * targetPath, char * logsPath, int port = 0, char* sh1Fingerpeint = NULL);
	static void checkifActionNeeded();
	static void updateServerLocalAction(char* action, int fingerprintId = -1);
};

