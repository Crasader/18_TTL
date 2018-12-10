#include "SGGameLogic.h"
#include "math.h"
#include "Game/FV/FvMask.h"

const byte SGGameLogic::GAME_CARDS[MAX_CARD_COUNT] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, //方块
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, //梅花
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, //红桃
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, //黑桃
};

const std::string SGGameLogic::GAME_CARDS_NAME[MAX_CARD_COUNT] = {
	"方块1", "方块2", "方块3", "方块4", "方块5", "方块6", "方块7", "方块8", "方块9", "方块10", "方块J", "方块Q", "方块K", //方块
	"梅花1", "梅花2", "梅花3", "梅花4", "梅花5", "梅花6", "梅花7", "梅花8", "梅花9", "梅花10", "梅花J", "梅花Q", "梅花K", //梅花
	"红桃1", "红桃2", "红桃3", "红桃4", "红桃5", "红桃6", "红桃7", "红桃8", "红桃9", "红桃10", "红桃J", "红桃Q", "红桃K", //红桃
	"黑桃1", "黑桃2", "黑桃3", "黑桃4", "黑桃5", "黑桃6", "黑桃7", "黑桃8", "黑桃9", "黑桃10", "黑桃J", "黑桃Q", "黑桃K", //黑桃
};

void SGGameLogic::initCard(byte cbCardData[], byte cbMaxCount)
{
	for (int index = 0; index < cbMaxCount && index < MAX_CARD_COUNT; ++index) {
		cbCardData[index] = GAME_CARDS[index];
	}
}

void SGGameLogic::RandCardData(byte cbCardData[], byte cbMaxCount)
{
	std::vector<byte> cardTemp;

	for (int index = 0; index < cbMaxCount; ++index) {
		cardTemp.push_back(cbCardData[index]);
	}

	for (int index = 0; index < cbMaxCount; ++index) {
		int random = rand() % cardTemp.size();
		cbCardData[index] = cardTemp.at(random);
		cardTemp.erase(find(cardTemp.begin(), cardTemp.end(), cardTemp.at(random)));
	}
}

SG::SGCardType_Result SGGameLogic::checkSGType(byte cbCardData[],dword dwGameRuleIdex) {
	for (int index = 0; index < MAX_HAND_CARD; ++index) {
		ASSERT(isCardValid(cbCardData[index]));
	}

	SG::SGCardType_Result result;

	// 大三公
	{
		bool isDSG = true;
		for (int index = 0; index < MAX_HAND_CARD; ++index) {
			if ((cbCardData[index] & MASK_VALUE) != 3) {
				isDSG = false;
				break;
			}
		}

		if (isDSG) {
			result.type = SG::SGCardType_DSG;
			result.cardPoint = 9;
			return result;
		}
	}

	//豹子-小三公
	{
		bool isBZ = true;
		byte tempCardValue = 0;
		for (int index = 0; index < MAX_HAND_CARD; index++) {
			if (tempCardValue == 0) {
				tempCardValue = cbCardData[index] & MASK_VALUE;
				continue;
			}

			if (tempCardValue != (cbCardData[index] & MASK_VALUE)) {
				isBZ = false;
				break;
			}
		}

		if (isBZ) {
			result.type = SG::SGCardType_BZ;
			if (tempCardValue <= 0xa) {
				result.type = SG::SGCardType_XSG;
			}
			result.cardPoint = tempCardValue;
			result.comparePoint = tempCardValue;
			return result;
		}
	}

	// 混三公-单牌
	{
		bool isHSG = true;
		byte totalCardValue = 0;
		byte tempCardValue = 0;
		byte tempCard[MAX_HAND_CARD];
		memcpy(tempCard, cbCardData, sizeof(tempCard));

		for (int index = 0; index < MAX_HAND_CARD - 1; ++index) {
			for (int index_sort = index + 1; index_sort < MAX_HAND_CARD; ++index_sort) {
				if ((tempCard[index] & MASK_VALUE) < (tempCard[index_sort] & MASK_VALUE)) {
					std::swap(tempCard[index], tempCard[index_sort]);
				}
			}
		}

		std::ostringstream compareCardPointStr;
		for (int index = 0; index < MAX_HAND_CARD; ++index) {
			tempCardValue = tempCard[index] & MASK_VALUE;
			if (tempCardValue <= 0x0a) {
				isHSG = false;
			}

			if (tempCardValue >= 0xa) {
				tempCardValue = 10;
			}

			compareCardPointStr << ((int)(tempCard[index] & MASK_VALUE) < 10 ? "0" : "") << (int)(tempCard[index] & MASK_VALUE);
			totalCardValue += tempCardValue;
		}

		if (isHSG) {
			result.type = SG::SGCardType_HSG;
		} else {
			result.type = SG::SGCardType_None;
		}

		result.cardPoint = totalCardValue % 10;
		std::istringstream outStr;
		outStr.str(compareCardPointStr.str());
		outStr >> result.comparePoint;

		return result;
	}
}

bool SGGameLogic::isCardValid(byte cardData)
{
	if ((cardData & MASK_COLOR) > 0x30) {
		return false;
	}

	if ((cardData & MASK_VALUE) == 0 || (cardData & MASK_VALUE) > 0x0d) {
		return false;
	}

	return true;
}

bool SGGameLogic::hasRule(dword gameRule, byte rule)
{
	return FvMask::HasAny(gameRule, _MASK_(rule));
}

int SGGameLogic::getCardIndex(byte cardValue)
{
	return (cardValue / 0x10) * 13 + (cardValue & 0xf);
}

int SGGameLogic::getSGRatio(SG::SGCardType_Result result) {
	switch (result.type) {
	case SG::SGCardType_None: {
		if (result.cardPoint > 7) {
			return 2;
		}
		return 1;
						  }
	case SG::SGCardType_HSG:
		return 3;
	case SG::SGCardType_XSG:
	case SG::SGCardType_BZ:
		return 4;
	case SG::SGCardType_DSG:
		return 5;
	default:
		break;
	}
	return 0;
}
