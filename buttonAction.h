#pragma once
class ButtonAction
{
public:
	virtual void onShortPress() = 0;
	virtual void onLongPress() = 0;
};
