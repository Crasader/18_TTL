#include <iostream>   
#include <map>
#include <cocos2d.h>
#include "Socket.h"
#include "Tools/utilityLog.h"

using namespace gamelib;

CSocket::CSocket()
	: mSocketSink(0)
{
}

CSocket::~CSocket()
{
}

void CSocket::setSocketSink(ISocketSink* pISocketSink)
{
	mSocketSink = pISocketSink;
}

bool CSocket::isAlive()
{
	return gamelib::MLClientNetDelegate::isConnected();
}

int CSocket::send(const char* data, int size)
{
	if (!isAlive())
		return -1;

	gamelib::MLClientNetDelegate::send(data,size);
	return 1;
}

int CSocket::connect(const char* url, int port)
{
	disconnect();
	m_oInetAddress.setIp(url);
	m_oInetAddress.setPort(port);

	gamelib::MLClientNetDelegate::connect();
	return 0;
}

void CSocket::disconnect()
{
	if (!isAlive())
		return;
	gamelib::MLClientNetDelegate::disconnect();
}

//////////////////////////////////////////////////////////////////////////
// 辅助函数
void CSocket::onConnected()
{
	if (mSocketSink)
		mSocketSink->onSocketLink();
}

void CSocket::onConnectTimeout()
{
	utility::filelog("CSocket::onConnectTimeout %s.", mSocketSink == 0 ? "null" : "no null");
	if (mSocketSink)
		mSocketSink->onSocketTimeOut();
}

void CSocket::onDisconnected()
{
	utility::filelog("CSocket::onDisconnected %s.", mSocketSink == 0 ? "null" : "no null");
	if (mSocketSink)
		mSocketSink->onSocketShut();
}

void CSocket::onExceptionCaught(CCSocketStatus eStatus)
{
	utility::filelog("CSocket::onExceptionCaught mSocketSink = %s.", mSocketSink == 0 ? "null" : "no null");
	if (mSocketSink)
		mSocketSink->onSocketError(eStatus);
}

void CSocket::onMessageReceived(char* pData,unsigned int iLenght)
{
	if (mSocketSink)
		mSocketSink->onSocketData(pData, iLenght);
}
