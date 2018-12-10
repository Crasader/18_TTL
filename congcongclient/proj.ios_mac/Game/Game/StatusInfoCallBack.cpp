#include "StatusInfoCallBack.h"

FV_SINGLETON_STORAGE(StatusInfoCenter);

StatusInfoCenter::StatusInfoCenter()
	: statusInterface(NULL)
{

}

StatusInfoCenter::~StatusInfoCenter()
{
}

void StatusInfoCenter::setStatusListener(StatusInfoInterface *pListener)
{
	statusInterface = pListener;
}

void StatusInfoCenter::setBattery(std::string level)
{
	if (statusInterface == NULL)
	{
		return;
	}
	statusInterface->onBatteryChange(level);
}

void StatusInfoCenter::setSignal(std::string status)
{
	if (statusInterface == NULL)
	{
		return;
	}
	statusInterface->onSignalChange(status);
}
