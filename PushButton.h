#pragma once
#include "buttonAction.h"
#include "NodeMcuConstants.h"
#include <Bounce2.h>
#include "myLogger.h"
class PushButton
{
private:
	ButtonAction *component;
	int pressTimeMs;
	int longpress;
	int shortpress;
	Bounce debouncer;
public:
	PushButton(PinConection pin, ButtonAction *component, int shortpress,int longpress);
	void checkIfActionNeeded();
	~PushButton();
};
