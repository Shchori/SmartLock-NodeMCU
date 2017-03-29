
#include "Led.h"
#include "FingerPrintProxy.h"

/*
 Name:		SmartLock.ino
 Created:	3/28/2017 12:55:19 PM
 Author:	Nir Shchori
*/

// the setup function runs once when you press reset or power the board
Led* blue = NULL;
Led* red = NULL;

void setup() {
	
	blue = new Led(PIN_D0);
	blue = new Led(PIN_D1);
	FingerPrintProxy::init(PIN_D2, PIN_D3);
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	delay(100);
	FingerPrintProxy* finger = FingerPrintProxy::getInstance();
	if (!finger) {
		Serial.println("FingerprintSensor Err");
	}
	delay(100);
	int status = finger->EnrollFingerprint(1);
	if (status) {
		Serial.println("Enroll Err");
	}
	delay(100);
	int confidence;
	Serial.println("Waiting for valid finger to indentify");
	status = finger->getFingerprintIDez(confidence);
	while (status == -1)
	{
		status = finger->getFingerprintIDez(confidence);
	}
	if (status<0) {
		Serial.println("didn't found match");
	}
	delay(100);

	Serial.println("delete fingerprint 1");
	status = finger->deleteFingerprint(1);
	if (status) {
		Serial.println("delete Err");
	}
	delay(100);
	Serial.println("Waiting for valid finger to indentify");
	status = finger->getFingerprintIDez(confidence);
	while (status == -1)
	{
		status = finger->getFingerprintIDez(confidence);
	}
	if (status<0) {
		Serial.println("didn't found match");
	}
}
