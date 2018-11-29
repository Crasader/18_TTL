#pragma once

#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Script/ActionEx.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/utility.h"
#include "Game/Script/TimeNode.h"
#include "Game/Script/TimeManager.h"
#include "Game/Script/SoundFun.h"
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
#include "Game/Game/UserInfo.h"
#include "Game/Game/NoticeMsg.h"
#include "Game/Game/NoticeMsgBox.h"
#include "Game/Game/EffectScence.h"
#include "Game/Game/PopScence.h"
#include "Game/Game/GameManagerBase.h"
#include "GameNet/ImagicDownManager.h"

//是否进入房间立即准备
//#define ENTER_GTAME_STATE_READY
//是否进入游戏快速坐下
//#define ENABLE_QUIK_SIT_DOWN
//后台线程启用
//#define ENABLE_BACK_GROUND_THREAD
//定时发送心跳包
//#define ENABLE_SEND_HEART_TICK_TIMER