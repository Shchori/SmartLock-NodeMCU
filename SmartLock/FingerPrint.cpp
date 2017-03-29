#include "FingerPrintProxy.h"



FingerPrintProxy* FingerPrintProxy::instance = NULL;

SoftwareSerial* FingerPrintProxy::serialConnection = NULL;

void FingerPrintProxy::init(PinConection pinIn, PinConection pinOut)
{
	serialConnection = new SoftwareSerial(pinIn, pinOut);
	if (!instance) {
		Serial.begin(9600);
		Serial.println("fingertest");
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
	if (serialConnection && instance->fingerPrint.verifyPassword()) {
		Serial.begin(9600);
		Serial.println("Found fingerprint sensor! ");
	}
	else {
		Serial.println("Did not find fingerprint sensor :(");
		FingerPrintProxy * temp;
		delete temp;
		instance = NULL;
	}
	return instance;
}


uint8_t FingerPrintProxy::EnrollFingerprint(uint8_t id) {
	uint8_t p = -1;
	Serial.begin(9600);
	Serial.println("Waiting for valid finger to enroll");
	while (p != FINGERPRINT_OK) {
		p = fingerPrint.getImage();
		switch (p) {
		case FINGERPRINT_OK:
			Serial.println("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			Serial.print(".");
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			Serial.println("Communication error");
			break;
		case FINGERPRINT_IMAGEFAIL:
			Serial.println("Imaging error");
			break;
		default:
			Serial.println("Unknown error");
			break;
		}
	}

	// OK success!

	p = fingerPrint.image2Tz(1);
	switch (p) {
	case FINGERPRINT_OK:
		Serial.println("Image converted");
		break;
	case FINGERPRINT_IMAGEMESS:
		Serial.println("Image too messy");
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		Serial.println("Communication error");
		return p;
	case FINGERPRINT_FEATUREFAIL:
		Serial.println("Could not find fingerprint features");
		return p;
	case FINGERPRINT_INVALIDIMAGE:
		Serial.println("Could not find fingerprint features");
		return p;
	default:
		Serial.println("Unknown error");
		return p;
	}

	Serial.println("Remove finger");
	delay(2000);
	p = 0;
	while (p != FINGERPRINT_NOFINGER) {
		p = fingerPrint.getImage();
	}

	p = -1;
	Serial.println("Place same finger again");
	while (p != FINGERPRINT_OK) {
		p = fingerPrint.getImage();
		switch (p) {
		case FINGERPRINT_OK:
			Serial.println("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			Serial.print(".");
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			Serial.println("Communication error");
			break;
		case FINGERPRINT_IMAGEFAIL:
			Serial.println("Imaging error");
			break;
		default:
			Serial.println("Unknown error");
			break;
		}
	}

	// OK success!

	p = fingerPrint.image2Tz(2);
	switch (p) {
	case FINGERPRINT_OK:
		Serial.println("Image converted");
		break;
	case FINGERPRINT_IMAGEMESS:
		Serial.println("Image too messy");
		return p;
	case FINGERPRINT_PACKETRECIEVEERR:
		Serial.println("Communication error");
		return p;
	case FINGERPRINT_FEATUREFAIL:
		Serial.println("Could not find fingerprint features");
		return p;
	case FINGERPRINT_INVALIDIMAGE:
		Serial.println("Could not find fingerprint features");
		return p;
	default:
		Serial.println("Unknown error");
		return p;
	}


	// OK converted!
	p = fingerPrint.createModel();
	if (p == FINGERPRINT_OK) {
		Serial.println("Prints matched!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Serial.println("Communication error");
		return p;
	}
	else if (p == FINGERPRINT_ENROLLMISMATCH) {
		Serial.println("Fingerprints did not match");
		return p;
	}
	else {
		Serial.println("Unknown error");
		return p;
	}

	p = fingerPrint.storeModel(id);
	if (p == FINGERPRINT_OK) {
		Serial.println("Stored!");
		return p;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Serial.println("Communication error");
		return p;
	}
	else if (p == FINGERPRINT_BADLOCATION) {
		Serial.println("Could not store in that location");
		return p;
	}
	else if (p == FINGERPRINT_FLASHERR) {
		Serial.println("Error writing to flash");
		return p;
	}
	else {
		Serial.println("Unknown error");
		return p;
	}
}


uint8_t FingerPrintProxy::deleteFingerprint(uint8_t id) {
	uint8_t p = -1;
	Serial.begin(9600);
	p = fingerPrint.deleteModel(id);

	if (p == FINGERPRINT_OK) {
		Serial.println("Deleted!");
		return p;
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Serial.println("Communication error");
		return p;
	}
	else if (p == FINGERPRINT_BADLOCATION) {
		Serial.println("Could not delete in that location");
		return p;
	}
	else if (p == FINGERPRINT_FLASHERR) {
		Serial.println("Error writing to flash");
		return p;
	}
	else {
		Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
		return p;
	}
}


int FingerPrintProxy::getFingerprintIDez(int& confidence) {
	uint8_t p = fingerPrint.getImage();
	if (p != FINGERPRINT_OK)  return -1;

	p = fingerPrint.image2Tz();
	if (p != FINGERPRINT_OK)  return -1;

	p = fingerPrint.fingerFastSearch();
	if (p != FINGERPRINT_OK)  return -2;

	// found a match!
	Serial.print("Found ID #"); Serial.print(fingerPrint.fingerID);
	Serial.print(" with confidence of "); Serial.println(fingerPrint.confidence);
	confidence = fingerPrint.confidence;
	return fingerPrint.fingerID;
}