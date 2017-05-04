#include "updateManager.h"

bool updateManager::connected = false;
bool updateManager::_init = false;
Lock* updateManager::lock = NULL;
FingerPrintProxy* updateManager::fingerprint = NULL;

RestClient* updateManager::restClient = NULL;



void updateManager::checkConnection() {
	if (!WiFi.isConnected()) {
		if (connected) {
			Logger::info("Disconnected\r\n");
			delay(20);
			connected = false;
		}
		WiFi.mode(WIFI_STA);
	}
	else {
		if (!connected )
		{
			Logger::info("Connected\r\n");
			delay(20);
			connected = true;
		}
	}



}

void updateManager::init(FingerPrintProxy * _fingerprint, Lock * _lock, const char* serverURL,int port)
{
	fingerprint = _fingerprint;
	lock = _lock;
	_init = true;
	mac = WiFi.macAddress().c_str();
	if (WiFi.hostname("SmartLock")) {
		Logger::info("host name set");
	}
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	restClient = new RestClient(serverURL,port);

	Logger::info("update manger was init MAC : "); 
	Logger::debug(mac);
}

void updateManager::checkifActionNeeded()
{
	checkConnection();
	if (connected && _init) {
		
	}
}
