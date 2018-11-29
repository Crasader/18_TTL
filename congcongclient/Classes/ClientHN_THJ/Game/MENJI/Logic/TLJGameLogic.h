#pragma once
#include "../TLJHeaders.h"
#include "../CMD_TLJ.h"

using namespace TLJ;

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��ɫ����
#define TYPE_BLOCK					0x00								//����
#define TYPE_PLUM					0x10								//÷��
#define TYPE_HEART					0x20								//����
#define TYPE_SPADE					0x30								//����

//�˿�����
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define	CT_SHUN_ZI					3									//˳������
#define CT_JIN_HUA					4									//������
#define	CT_SHUN_JIN					5									//˳������
#define	CT_BAO_ZI					6									//��������
#define CT_SPECIAL					7									//��������

namespace TLJ
{
	class TLJGameLogic: public FvSingleton<TLJGameLogic>
	{		
		//��������
	private:
		static BYTE						m_cbCardListData[52];				//�˿˶���
		//��������
	public:
		//���캯��
		TLJGameLogic();
		//��������
		virtual ~TLJGameLogic();
	public:
		//��ȡ��ֵ
		BYTE GetCardValue(BYTE cbCardData);
		//��ȡ��ɫ
		BYTE GetCardColor(BYTE cbCardData);
		//��ȡ��ʾ��Դ��
		std::string GetShowCardValue(BYTE cbCardData);
		//��Ч�ж�
		bool IsValidCard(BYTE cbCardData);
	public:
		WORD switchToScore(BYTE cbLevel);
		BYTE switchToLevel(WORD wScore);

		//����Ͷע����
		DWORD GetBettingScore(BYTE cbScoreLevel,bool bMingPai, WORD wTimes, DWORD dwBaseScore);
	};
}

