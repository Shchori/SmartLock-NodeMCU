
/*
Name:		SmartLock.ino
Created:	3/28/2017 12:55:19 PM
Author:	Nir Shchori
*/


#include "NodeMcuConstants.h"
#include "PushButton.h"
#include "Lock.h"
#include "LockButton.h"
#include "WifiConfigurationButton.h"
#include "FingerPrintProxy.h"
#include "myLogger.h"
#include "updateManager.h"


int shortPress = 100;
int longpress = 2500;

PushButton* lockPushButton = NULL;
PushButton* wifiConfigButton = NULL;
Lock* lock = NULL;
FingerPrintProxy* fingerprint = NULL;
bool connected = true;


void setup() {
	Serial.begin(115200);
	Logger::info("info: ****************\r\n");
	Logger::info("info: SmartLock Start\r\n");
	Logger::info("info: ********************\r\n");
	WifiConfigurationButton* wifiB = new WifiConfigurationButton();
	Lock* lock = new Lock(PIN_D0, PIN_D3);
	FingerPrintProxy::init(PIN_D1, PIN_D2);
	fingerprint = FingerPrintProxy::getInstance();
	ButtonAction* lockbutton = new LockButton(lock, fingerprint);
	lockPushButton = new PushButton(PIN_D6, lockbutton, longpress, shortPress);
	ButtonAction* wifibutton = new WifiConfigurationButton();
	wifiConfigButton = new PushButton(PIN_D5, wifibutton, longpress, shortPress);
	updateManager::init(fingerprint, lock, "localhost", 3000);
}

// the loop function runs over and over again until power down or reset
void loop() {
	updateManager::checkifActionNeeded();
	lockPushButton->checkIfActionNeeded();
	wifiConfigButton->checkIfActionNeeded();
}

