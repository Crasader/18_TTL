#ifndef _CARD_OPTION_H_
#define _CARD_OPTION_H_

namespace cardturn {
//扑克宽高比
#define CARD_PROPOTION 0.6477247f
//裁剪形状的顶点数
#define CLIP_VERTEXT_SIZE 4
//强制移动时的移动距离
#define FORCE_STEP_VALUE 8.0f
//每一步移动像素大小
#define ROLL_BACK_STEP_VALUE 6.0f
//圆周率
#define PI 3.14159265

//搓牌的状态
enum CardTurn_Action_State {
	CTAS_Null,
	CTAS_Touched,
	CTAS_Moved,
	CTAS_Reset,
	CTAS_ForceComplete,//强制结束状态
	CTAS_Complete,
	CTAS_End,
};

//搓牌的方向
enum CardTurn_Direction {
	CTD_BoomRight_To_TopLeft,
	CTD_Boom_To_Up,
	CTD_Left_To_Right,
};

//搓牌完结方式
enum CardTurn_Complete_Action_Type {
	CTCAT_Nomal,//搓完结束
	CTCAT_ForceEndOverHalf,//搓到一半跳牌
};

typedef void* cardTurnCompleteFunc(void* param);

}

#endif