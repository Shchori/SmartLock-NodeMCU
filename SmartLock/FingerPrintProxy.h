#pragma once
#include <Adafruit_Fingerprint.h>
#include "NodeMcuConstants.h"
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
	 uint8_t EnrollFingerprint(uint8_t id);
	 uint8_t deleteFingerprint(uint8_t id);
	 int getFingerprintIDez(int& confidence);
};

