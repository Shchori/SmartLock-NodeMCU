#include "updateManager.h"

Lock* updateManager::lock = NULL;
FingerPrintProxy* updateManager::fingerprint = NULL;
RestClient* updateManager::restClient = NULL;
char * updateManager::mac = NULL;
char * updateManager::path = NULL;
char * updateManager::logPath = NULL;
unsigned long updateManager::nextUpdate = millis();
bool updateManager::_init = false;
int updateManager::serverErrorTime = 0;

void updateManager::init(FingerPrintProxy* _fingerprint, Lock* _lock, const char* serverURL, char * targetPath, char * logsPath, int port , char* sh1Fingerpeint)
{
	fingerprint = _fingerprint;
	lock = _lock;
	char * pathArray = new char[100];
	path = pathArray;
	char * logsPathArray = new char[100];
	logPath = logsPathArray;
	Logger::info("server :"); Serial.print(serverURL);delay(40);
	mac = WifiConnectManager::getMacAdress(); //"18:fe:34:d4:c6:e8"; // 
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
	sprintf(logsPathArray, "%s/%s/", logsPath, mac);
	Logger::info("logs path : "); Serial.print(logsPathArray); delay(40);
	Logger::info("update manger was init MAC : "); Serial.print(mac); delay(40);
	_init = true;
}

void updateManager::checkifActionNeeded()
{
	if (nextUpdate < millis()) {
		if (WifiConnectManager::checkConnection() && _init) {
			String response = "";
			if (serverErrorTime == 3) {
				serverErrorTime = 0;
			}
			Logger::info("ask for update");
			Logger::debug("Send get request");
			Serial.begin(9600);
			int statusCode = restClient->get(path, &response);
			Serial.begin(115200);
			delay(50);
			if (statusCode == 200) {
				serverErrorTime = 0;
				Logger::info("get response");
				Logger::debug("request status :"); Serial.println(statusCode);
				Logger::debug("##response :\n"); Serial.println(response); delay(20);
				handleResponse(response);
				nextUpdate = millis() + UPDATE_TIME * 1000;
			}
			else {
				serverErrorTime++;
				Logger::debug("Failed request status :"); Serial.println(statusCode);
				Logger::debug("##response :\n"); Serial.println(response); delay(20);
				Logger::debug("Error counter "); Serial.println(serverErrorTime); delay(20);
				if (serverErrorTime < 3) {
					nextUpdate = millis() + 3 * 1000;
				}
				else {
					nextUpdate = millis() + 180 * 1000;

				}
			}
		}
	}
}
void updateManager::updateServerLocalAction(char* action, int fingerprintId) {
	if (WifiConnectManager::checkConnection() && _init && serverErrorTime<3) {
		Logger::info("update the server about :"); Serial.print(action); delay(20);
		if (!strcmp(action, "lock") || !strcmp(action, "unlock")) {
			int responseCode;
			String body;
			StaticJsonBuffer<200> postbuffer;
			JsonObject& rootReqBody = postbuffer.createObject();
			rootReqBody["action"] = action;
			if (fingerprintId > -1) {
				rootReqBody["fingerId"] = fingerprintId;
			}
			rootReqBody.prettyPrintTo(body);
			restClient->setContentType("application/json");
			Logger::debug("send request to server to write log to path:"); Serial.print(logPath); delay(20);
			Logger::debug("\nbody :\n"); Serial.print(body.c_str()); delay(20);
			Serial.begin(9600);
			responseCode = restClient->post(logPath, body.c_str());
			Serial.begin(115200);
			if (responseCode != 200) {
				Logger::info("status code Error!!!!"); 
				serverErrorTime++;
				Logger::debug("Error counter"); Serial.println(serverErrorTime); delay(20);
			}
			Logger::debug("status code: "); Serial.print(responseCode); delay(20);

		}
		else
		{
			Logger::info("unillegel action");
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
		JsonObject& rootReqBody = postbuffer.createObject();
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
		Serial.begin(9600);
		responseCode = restClient->post(path, body.c_str());
		Serial.begin(115200);
		Logger::info("status code: "); Serial.print(responseCode);
	}
};