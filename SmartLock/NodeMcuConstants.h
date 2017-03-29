#pragma once
//nodeMcu constants
enum PinConection
{
	PIN_D0 = 16,
	PIN_D1 = 5, // I2C Bus SCL (clock)
	PIN_D2 = 4, // I2C Bus SDA (data)
	PIN_D3 = 0,
	PIN_D4 = 2, // Same as "LED_BUILTIN", but inverted logic
	PIN_D5 = 14, // SPI Bus SCK (clock)
	PIN_D6 = 12, // SPI Bus MISO 
	PIN_D7 = 13, // SPI Bus MOSI
	PIN_D8 = 15, // SPI Bus SS (CS)
	PIN_D9 = 3, // RX0 (Serial console)
	PIN_D10 = 1 // TX0 (Serial console)
};

enum PinMode {
	IN = INPUT,
	OUT = OUTPUT 
};

enum PinStatus {
	ON = HIGH,
	OFF = LOW
};



