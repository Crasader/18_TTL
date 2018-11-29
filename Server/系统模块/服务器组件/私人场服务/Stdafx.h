#pragma once

//////////////////////////////////////////////////////////////////////////

#pragma warning ( disable : 4996)
#pragma warning ( disable : 4005)

//////////////////////////////////////////////////////////////////////////




#ifndef VC_EXTRALEAN

#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料

#endif



// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。

// 有关不同平台的相应值的最新信息，请参考 MSDN。

#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。

#define WINVER 0x0502		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。

#endif



#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。

#define _WIN32_WINNT 0x0502	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。

#endif						



#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。

#define _WIN32_WINDOWS 0x0410 // 将此更改为针对于 Windows Me 或更高版本的合适的值。

#endif



#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。

#define _WIN32_IE 0x0400	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。

#endif



#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的



#include <afxwin.h>         // MFC 核心组件和标准组件

#include <afxext.h>         // MFC 扩展



#ifndef _AFX_NO_OLE_SUPPORT

#include <afxole.h>         // MFC OLE 类

#include <afxodlgs.h>       // MFC OLE 对话框类

#include <afxdisp.h>        // MFC 自动化类

#endif // _AFX_NO_OLE_SUPPORT



#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持

#ifndef _AFX_NO_AFXCMN_SUPPORT

#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持

#endif // _AFX_NO_AFXCMN_SUPPORT



//////////////////////////////////////////////////////////////////////////



//组件头文件


#include "..\..\服务器组件\游戏服务\GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////
//链接代码

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Ansi/GameService.lib")
#pragma comment (lib,"../../链接库/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Unicode/GameService.lib")
#pragma comment (lib,"../../链接库/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Ansi/GameServiceD.lib")
#pragma comment (lib,"../../链接库/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Unicode/GameServiceD.lib")
#pragma comment (lib,"../../链接库/Unicode/KernelEngineD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

//是否可以游戏中离开，不发解散请求
//#define ALLOWED_QUIT_ROOM_INGAME_WITHOUT_AGREE
//否允许创建空房间模式,   房主创建房间后可以离开
//#define ALLOWED_CREATE_EMPTY_ROOM
//如果是无限制开放模式,   无需任何消耗
//#define ALLOWED_CREATE_ROOM_NO_COST
//允许同一个房间无限次游戏
//#define ALLOWED_PLAY_COUNT_UNLIMITED
//房卡场显示金币或者显示输赢
//#define ROOM_ONLY_COST_GOLD
//等待踢人开关
//#define ALLOWED_KICK_UNREADY_USER
//使用抽水
//#define ALLOWED_REBATE
//聪聪平台的狗屎规则
#define PLATFORM_CONGCONG
//连接支付服务器开关
//#define ALLOWED_CONECT_PAYMENT

//////////////////////////////////////////////////////////////////////////