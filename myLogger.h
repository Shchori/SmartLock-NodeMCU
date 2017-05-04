#ifndef  LOGGER
#define LOGGER

#include <stdarg.h>
class Logger
{
public:

	static void info(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		delay(50);
		Serial.print("\n##info## : ");
		Serial.printf(fmt, args);
		delay(50);
		va_end(args);
	};

	static void debug(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		delay(50);
		Serial.print("\n##Debug## : ");
		Serial.printf(fmt, args);
		delay(50);
		va_end(args);
	};

};


#endif // ! LOGGER
