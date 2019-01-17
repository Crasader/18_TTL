#ifndef _CARD_OPTION_H_
#define _CARD_OPTION_H_

namespace cardturn {
//�˿˿�߱�
#define CARD_PROPOTION 0.6477247f
//�ü���״�Ķ�����
#define CLIP_VERTEXT_SIZE 4
//ǿ���ƶ�ʱ���ƶ�����
#define FORCE_STEP_VALUE 8.0f
//ÿһ���ƶ����ش�С
#define ROLL_BACK_STEP_VALUE 6.0f
//Բ����
#define PI 3.14159265

//���Ƶ�״̬
enum CardTurn_Action_State {
	CTAS_Null,
	CTAS_Touched,
	CTAS_Moved,
	CTAS_Reset,
	CTAS_ForceComplete,//ǿ�ƽ���״̬
	CTAS_Complete,
	CTAS_End,
};

//���Ƶķ���
enum CardTurn_Direction {
	CTD_BoomRight_To_TopLeft,
	CTD_Boom_To_Up,
	CTD_Left_To_Right,
};

//������᷽ʽ
enum CardTurn_Complete_Action_Type {
	CTCAT_Nomal,//�������
	CTCAT_ForceEndOverHalf,//�굽һ������
};

typedef void* cardTurnCompleteFunc(void* param);

}

#endif