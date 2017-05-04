#include "PushButton.h"


PushButton::PushButton(PinConection pin,ButtonAction* component, int longpress,int shortpress)
{

	pinMode(pin, INPUT_PULLUP);
	this->component = component;
	this->debouncer = Bounce();
	this->shortpress = shortpress;
	this->longpress = longpress;
	debouncer.attach(pin); //define pin button output
	debouncer.interval(5); //define press time
	Logger::info("Create Push Button");
	Logger::debug("Button pin "); Serial.print(pin);
}

void PushButton::checkIfActionNeeded()
{
	debouncer.update();
	Serial.begin(115200);
	//Serial.println("Check if button was press: ");
	if (debouncer.rose()) {
		this->pressTimeMs = millis();
		debouncer.update();
		Logger::debug("button was pressed wait until the button will be release");
		while (debouncer.read()) {
			//wait until button will be release
			debouncer.update();
			delay(50);
		}	
		int pressTimeLong = millis() - pressTimeMs;
		Logger::debug("button was released ");
		Serial.println(pressTimeLong);
		if (pressTimeLong >= this->longpress) {
			Logger::info("execute long press ");
			this->component->onLongPress();
		}
		else if (pressTimeLong >= this->shortpress) {
			Logger::info("execute short press ");
			this->component->onShortPress();
		}
	}

}

PushButton::~PushButton()
{
	delete this->component;
}
