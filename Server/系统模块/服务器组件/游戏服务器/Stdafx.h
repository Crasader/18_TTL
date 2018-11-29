#pragma once

//////////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4996)
#pragma warning (disable : 4005)

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//MFC 文件

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxDisp.h>
#include <AfxDtctl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//平台定义
#include "..\..\全局定义\Platform.h"
#include "..\..\消息定义\CMD_Correspond.h"
#include "..\..\消息定义\CMD_GameServer.h"
#include "..\..\消息定义\CMD_LogonServer.h"
#include "..\..\消息定义\CMD_PaymentServer.h"

//组件定义
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "..\..\服务器组件\游戏服务\GameServiceHead.h"
#include "..\..\服务器组件\比赛服务\MatchServiceHead.h"
#include "..\..\服务器组件\私人场服务\PrivateServiceHead.h"
#include "..\..\服务器组件\内核引擎\KernelEngineHead.h"
#include "..\..\服务器组件\模块管理\ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////
//链接代码

#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../链接库/Ansi/GameService.lib")
	#pragma comment (lib,"../../链接库/Ansi/KernelEngine.lib")
	#pragma comment (lib,"../../链接库/Ansi/ModuleManager.lib")
#else
	#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../链接库/Unicode/GameService.lib")
	#pragma comment (lib,"../../链接库/Unicode/KernelEngine.lib")
	#pragma comment (lib,"../../链接库/Unicode/ModuleManager.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../链接库/Ansi/GameServiceD.lib")
	#pragma comment (lib,"../../链接库/Ansi/KernelEngineD.lib")
	#pragma comment (lib,"../../链接库/Ansi/ModuleManagerD.lib")
#else
	#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../链接库/Unicode/GameServiceD.lib")
	#pragma comment (lib,"../../链接库/Unicode/KernelEngineD.lib")
	#pragma comment (lib,"../../链接库/Unicode/ModuleManagerD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

//否允许创建空房间模式,   房主创建房间后可以离开
//#define ALLOWED_CREATE_EMPTY_ROOM
//是否可以游戏中离开，不发解散请求
//#define ALLOWED_QUIT_ROOM_INGAME_WITHOUT_AGREE
//是否踢出不准备的房主
//#define KICK_ROOM_OWNER
//如果是无限制开放模式,   无需任何消耗
//#define ALLOWED_CREATE_ROOM_NO_COST
//使用抽水
//#define ALLOWED_REBATE
//聪聪平台的狗屎规则
#define PLATFORM_CONGCONG
//连接支付服务器开关
//#define ALLOWED_CONECT_PAYMENT
//房卡模式
#define ENABLE_ROOM_CARD_TYPE
//////////////////////////////////////////////////////////////////////////
