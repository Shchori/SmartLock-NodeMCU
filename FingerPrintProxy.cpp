#include "FingerPrintProxy.h"



FingerPrintProxy* FingerPrintProxy::instance = NULL;

SoftwareSerial* FingerPrintProxy::serialConnection = NULL;

void FingerPrintProxy::init(PinConection pinIn, PinConection pinOut)
{
	serialConnection = new SoftwareSerial(pinIn, pinOut);
	Logger::info("config fingerprint sensor");
	Logger::debug("input pin : "); Serial.print(pinIn);
	Logger::debug("output pin : "); Serial.print(pinOut);
	if (!instance) {
		// set the data rate for the sensor serial port
		getInstance();
	}

}

FingerPrintProxy::FingerPrintProxy(SoftwareSerial * serialConnection):fingerPrint(serialConnection){}

FingerPrintProxy* FingerPrintProxy::getInstance()
{
	if (!instance) {
		instance = new FingerPrintProxy(serialConnection);
		instance->fingerPrint.begin(57600);
	}
	Logger::info("check fingerprint sensor");
	if (serialConnection && instance->fingerPrint.verifyPassword()) {
		Serial.begin(115200);
		Logger::info("Found fingerprint sensor! ");
	}
	else {
		Logger::info("Did not find fingerprint sensor :(");
		FingerPrintProxy * temp;
		delete temp;
		instance = NULL;
	}
	return instance;
}


int FingerPrintProxy::EnrollFingerprint(uint8_t id) {
	uint8_t p = -1;
	Serial.begin(115200);
	Logger::info("Start enroll fingerprint id:"); Serial.print(id);
	Logger::info("Waiting for valid finger to enroll");
	int startTime = millis();
	while (p != FINGERPRINT_OK && (FINGERPRINT_TIMEOUT + startTime) > millis()) {
		p = fingerPrint.getImage();
		switch (p) {
		case FINGERPRINT_OK:
			Logger::info("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			Serial.print(".");
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			Logger::info("Communication error");
			break;
		case FINGERPRINT_IMAGEFAIL:
			Logger::info("Imaging error");
			break;
		default:
			Logger::info("Unknown error");
			break;
		}
	}
	if (p != FINGERPRINT_OK) {
		return -1;
	}
	// OK success!

	p = fingerPrint.image2Tz(1);
	switch (p) {
	case FINGERPRINT_OK:
		Logger::info("Image converted");
		break;
	case FINGERPRINT_IMAGEMESS:
		Logger::info("Image too messy");
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		Logger::info("Communication error");
		return p;
	case FINGERPRINT_FEATUREFAIL:
		Logger::info("Could not find fingerprint features");
		return p;
	case FINGERPRINT_INVALIDIMAGE:
		Logger::info("Could not find fingerprint features");
		return p;
	default:
		Logger::info("Unknown error");
		return p;
	}
	if (p != FINGERPRINT_OK) {
		return -1;
	}
	Logger::info("Remove finger");
	delay(2000);
	p = 0;
	while (p != FINGERPRINT_NOFINGER) {
		p = fingerPrint.getImage();
	}

	p = -1;
	Logger::info("Place same finger again");
	startTime = millis();
	while (p != FINGERPRINT_OK && (FINGERPRINT_TIMEOUT + startTime) > millis()) {
		p = fingerPrint.getImage();
		switch (p) {
		case FINGERPRINT_OK:
			Logger::info("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			Serial.print(".");
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			Logger::info("Communication error");
			break;
		case FINGERPRINT_IMAGEFAIL:
			Logger::info("Imaging error");
			break;
		default:
			Logger::info("Unknown error");
			break;
		}
	}
	if (p != FINGERPRINT_OK) {
		return -1;
	}

	// OK success!

	p = fingerPrint.image2Tz(2);
	switch (p) {
	case FINGERPRINT_OK:
		Logger::info("Image converted");
		break;
	case FINGERPRINT_IMAGEMESS:
		Logger::info("Image too messy");
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		Logger::info("Communication error");
		return p;
	case FINGERPRINT_FEATUREFAIL:
		Logger::info("Could not find fingerprint features");
		return p;
	case FINGERPRINT_INVALIDIMAGE:
		Logger::info("Could not find fingerprint features");
		return p;
	default:
		Logger::info("Unknown error");
		return p;
	}


	// OK converted!
	p = fingerPrint.createModel();
	if (p == FINGERPRINT_OK) {
		Logger::info("Prints matched!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Logger::info("Communication error");
		return p;
	}
	else if (p == FINGERPRINT_ENROLLMISMATCH) {
		Logger::info("Fingerprints did not match");
		return p;
	}
	else {
		Logger::info("Unknown error");
		return p;
	}

	p = fingerPrint.storeModel(id);
	if (p == FINGERPRINT_OK) {
		Logger::info("Stored!");
		return p;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Logger::info("Communication error");
		return p;
	}
	else if (p == FINGERPRINT_BADLOCATION) {
		Logger::info("Could not store in that location");
		return p;
	}
	else if (p == FINGERPRINT_FLASHERR) {
		Logger::info("Error writing to flash");
		return p;
	}
	else {
		Logger::info("Unknown error");
		return p;
	}
}


int FingerPrintProxy::deleteFingerprint(uint8_t id) {
	uint8_t p = -1;
	Serial.begin(115200);
	Logger::info("Start delete fingerprint id : ");Serial.print(id);
	p = fingerPrint.deleteModel(id);

	if (p == FINGERPRINT_OK) {
		Logger::info("Deleted!");
		return p;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Logger::info("Communication error");
		return p;
	}
	else if (p == FINGERPRINT_BADLOCATION) {
		Logger::info("Could not delete in that location");
		return p;
	}
	else if (p == FINGERPRINT_FLASHERR) {
		Logger::info("Error writing to flash");
		return p;
	}
	else {
		Logger::info("Unknown error: 0x"); Logger::info("%04x", p);
		return p;
	}
}


int FingerPrintProxy::getFingerprintID() {
	uint8_t p = fingerPrint.getImage();
	int startTime = millis();
	Logger::info("wait for fingerprint....");
	while (p!=FINGERPRINT_OK && (FINGERPRINT_TIMEOUT+startTime) > millis() )
	{
		p = fingerPrint.getImage();
		delay(100);
	}
	if (p != FINGERPRINT_OK)  return -1;

	p = fingerPrint.image2Tz();
	if (p != FINGERPRINT_OK)  return -1;

	p = fingerPrint.fingerFastSearch();
	if (p != FINGERPRINT_OK) {
		Logger::info("not Found fingerprint");
		return -1;
	}
	// found a match!
	Logger::debug("Found ID # :");Serial.print( fingerPrint.fingerID);
	Logger::debug(" with confidence of : ");Serial.print(fingerPrint.confidence);
	if (fingerPrint.confidence > 70) {
		return fingerPrint.fingerID;
	}
	return - 1;
}