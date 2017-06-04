
/*
Name:		SmartLock.ino
Created:	3/28/2017 12:55:19 PM
Author:	Nir Shchori
*/


#include "NodeMcuConstants.h"
#include "PushButton.h"
#include "Lock.h"
#include "FingerPrintProxy.h"
#include "myLogger.h"
#include "updateManager.h"
#include "InDoorButton.h"
#include "OutDoorButton.h"
#include "WifiConnectManager.h"

PushButton* inDoorButton = NULL;
PushButton* outDoorButton = NULL;
Lock* lock = NULL;
FingerPrintProxy* fingerprint = NULL;
bool connected = true;
char * serverSH1ingerprint = "08 3B 71 72 02 43 6E CA ED 42 86 93 BA 7E DF 81 C4 BC 62 30";

void setup() {
	Serial.begin(115200);
	Logger::info("info: ****************\r\n");
	Logger::info("info: SmartLock Start\r\n");
	Logger::info("info: ********************\r\n");
	Lock* lock = new Lock(PIN_D8, PIN_D0,PIN_D7);
	FingerPrintProxy::init(PIN_D1, PIN_D2);
	WifiConnectManager::init(PIN_D3);
	updateManager::init(fingerprint, lock, "smartlockproject.herokuapp.com", "/api/lockRequest", 443, serverSH1ingerprint);
	fingerprint = FingerPrintProxy::getInstance();
	ButtonAction* outButton = new OutDoorButton(lock, fingerprint);
	ButtonAction* inButton = new InDoorButton(lock);
	outDoorButton = new PushButton(PIN_D6, outButton, 2.5 * 1000, 20);
	inDoorButton = new PushButton(PIN_D5, inButton, 10 * 1000, 20);
}

// the loop function runs over and over again until power down or reset
void loop() {
	updateManager::checkifActionNeeded();
	outDoorButton->checkIfActionNeeded();
	inDoorButton->checkIfActionNeeded();
}