#pragma once
#include <Adafruit_Fingerprint.h>
#include "myLogger.h"
#include "NodeMcuConstants.h"

#define CONFIDENCE 90
#define FINGERPRINT_TIMEOUT 9000

//this class is a proxy to use ADA fingerPrint sensor
class FingerPrintProxy
{
private:
	FingerPrintProxy(SoftwareSerial * serialConnection);
	static FingerPrintProxy * instance;
	Adafruit_Fingerprint fingerPrint;
	static  SoftwareSerial *serialConnection;
public:
	 static void init(PinConection, PinConection);
	 static FingerPrintProxy* getInstance();
	 int EnrollFingerprint(uint8_t id);
	 int deleteFingerprint(uint8_t id);
	 int getFingerprintID();
};

