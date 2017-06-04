#include "updateManager.h"

Lock* updateManager::lock = NULL;
FingerPrintProxy* updateManager::fingerprint = NULL;
RestClient* updateManager::restClient = NULL;
char * updateManager::mac = NULL;
char * updateManager::path = NULL;
unsigned long updateManager::nextUpdate = millis();
bool updateManager::_init = false;

void updateManager::init(FingerPrintProxy* _fingerprint, Lock* _lock, const char* serverURL, char * targetPath, int port , char* sh1Fingerpeint)
{
	fingerprint = _fingerprint;
	lock = _lock;
	char * pathArray = new char[100];
	path = pathArray;
	Logger::info("server :"); Serial.print(serverURL);delay(40);
	mac = WifiConnectManager::getMacAdress();
	if (port) {
		if (sh1Fingerpeint) {
			restClient = new RestClient(serverURL, port,sh1Fingerpeint);
			Logger::info("port :"); Serial.print(port);
			Logger::info("sh1Fingerpeint :"); Serial.print(sh1Fingerpeint);
			restClient->setSSL(1);
			delay(40);
		}
		else{
			restClient = new RestClient(serverURL, port);
			Logger::info("port :"); Serial.print(port);
		}
	}
	else {
		restClient = new RestClient(serverURL);
	}
	sprintf(pathArray,"%s/%s/" ,targetPath, mac);
	Logger::info("path : ");Serial.print(pathArray); delay(40);
	Logger::info("update manger was init MAC : "); Serial.print(mac); delay(40);
	_init = true;
}

void updateManager::checkifActionNeeded()
{
	if (nextUpdate < millis()) {
		if (WifiConnectManager::checkConnection() && _init) {
			String response = "";
			Logger::info("ask for update");
			Logger::debug("Send get request");
			int statusCode = restClient->get(path, &response);
			delay(50);
			if (statusCode == 200) {
				Logger::info("get response");
				Logger::debug("request status :"); Serial.println(statusCode);
				Logger::debug("##response :\n"); Serial.println(response); delay(50);
				handleResponse(response);
				nextUpdate = millis() + 7* 1000;
			}
			else {
				Logger::debug("Failed request status :"); Serial.println(statusCode);
				Logger::debug("##response :\n"); Serial.println(response); delay(50);
				nextUpdate = millis() + 3 * 1000;
			}

		}
	}
}

void updateManager::handleResponse(String json) {

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& rootRes = jsonBuffer.parseObject(json);
	const char* action = rootRes["action"];
	Logger::debug("request action: ", action); Serial.println(action); delay(20);
	if (!strcmp("no action needed", action)) {
		Logger::info("there is no update");
		return;
	}else {
		int responseCode;
		String body;
		const char* requestId = rootRes["requestId"];
		StaticJsonBuffer<200> postbuffer;
		JsonObject& rootReqBody = jsonBuffer.createObject();
		if (!strcmp(action, "unlock")) {
			Logger::info("get unlock request");
			lock->unlock();
			rootReqBody["status"] = "unlock";
		}
		else if (!strcmp(action, "lock")) {
			Logger::info("get lock request");
			lock->lock();
			rootReqBody["status"] = "lock";

		}
		else if (!strcmp(action, "checkStatus")) {
			Logger::info("get check Status request");
			if (lock->isLock()) {
				rootReqBody["status"] = "lock";
			}
			else {
				rootReqBody["status"] = "unlock";
			}

		}
		else if (!strcmp(action, "addFingerprint")) {
			int fingerprintId = rootRes["fingerprintId"];
			Logger::info("get add fingerprint request, id :"); Serial.print(fingerprintId);
			if (fingerprint->EnrollFingerprint(fingerprintId) >= 0) {
				rootReqBody["status"] = "done";
			}
			else {
				rootReqBody["status"] = "error";
			}
		}
		else if (!strcmp(action, "delFingerprint")) {
			int fingerprintId = rootRes["fingerprintId"];
			Logger::info("get del fingerprint request, id :"); Serial.print(fingerprintId);
			if (fingerprint->deleteFingerprint(fingerprintId) >= 0) {
				rootReqBody["status"] = "done";
			}
			else {
				rootReqBody["status"] = "error";
			}
		}
		else{ 
			Logger::info("unknow method");
			return; 
		}
		rootReqBody["requestId"] = requestId;
		rootReqBody.prettyPrintTo(body);
		restClient->setContentType("application/json");
		Logger::info("send response to server with body :\n"); Serial.print(body.c_str()); delay(20);
		responseCode = restClient->post(path, body.c_str());
		Logger::info("status code: "); Serial.print(responseCode);
	}
};