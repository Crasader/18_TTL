#pragma once
#include "Game\FV\FvSingleton.h"

class StatusInfoInterface {
public:
	virtual void onBatteryChange(std::string level) = 0;

	virtual void onSignalChange(std::string status) = 0;
};

class StatusInfoCenter
	: public FvSingleton<StatusInfoCenter>
{
public:
	StatusInfoCenter();
	~StatusInfoCenter();

	void setStatusListener(StatusInfoInterface *pListener);
	void setBattery(std::string level);
	void setSignal(std::string status);
private: 
	StatusInfoInterface * statusInterface;

};

