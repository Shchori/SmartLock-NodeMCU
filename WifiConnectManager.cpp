#include "WifiConnectManager.h"



bool WifiConnectManager::connected = false;
bool WifiConnectManager::_init = false;
Led* WifiConnectManager::disconnectedLed = NULL;

void WifiConnectManager::init(PinConection led) {
	disconnectedLed = new Led(led);
	if (WiFi.hostname("SmartLock")) {
		Logger::info("host name set");
	}
	checkConnection();
	_init = true;

}

bool WifiConnectManager::checkConnection() {
	if (!WiFi.isConnected()) {
		if (connected || !_init) {
			Logger::info("Disconnected\r\n");
			connected = false;
			disconnectedLed->turnON();
		}
		WiFi.mode(WIFI_STA);
	}
	if (WiFi.isConnected()) {
		if (!connected)
		{
			Logger::info("Connected\r\n");
			connected = true;
			disconnectedLed->flashTimes(2, 100);
			disconnectedLed->turnOFF();
		}
		return true;
	}
	return false;
}

char * WifiConnectManager::getMacAdress()
{
	char* mac = new char[30]();
	uint8_t MAC_array[6];
	WiFi.macAddress(MAC_array);
	for (int i = 0; i < sizeof(MAC_array) - 1; ++i) {
		sprintf(mac, "%s%02x:", mac, MAC_array[i]);
	}
	sprintf(mac, "%s%02x", mac, MAC_array[sizeof(MAC_array) - 1]);
	return mac;
}

void WifiConnectManager::resetSetting()
{
	Logger::info("reset setting and start portal to sec:"); Serial.print(PORTAL_TIME_OUT);
	WiFiManager wifiManager;
	wifiManager.resetSettings();
	delay(100);
	checkConnection();
	wifiManager.setConfigPortalTimeout(PORTAL_TIME_OUT);
	wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));
	wifiManager.autoConnect("SmartLock", "12345678");
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
}
