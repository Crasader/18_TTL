#include "NNGameLogic.h"
#include "math.h"
#include "Game/FV/FvMask.h"

const byte NNGameLogic::GAME_CARDS[MAX_CARD_COUNT] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, //·½¿é
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, //Ã·»¨
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, //ºìÌÒ
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, //ºÚÌÒ
};

const std::string NNGameLogic::GAME_CARDS_NAME[MAX_CARD_COUNT] = {
	"·½¿é1", "·½¿é2", "·½¿é3", "·½¿é4", "·½¿é5", "·½¿é6", "·½¿é7", "·½¿é8", "·½¿é9", "·½¿é10", "·½¿éJ", "·½¿éQ", "·½¿éK", //·½¿é
	"Ã·»¨1", "Ã·»¨2", "Ã·»¨3", "Ã·»¨4", "Ã·»¨5", "Ã·»¨6", "Ã·»¨7", "Ã·»¨8", "Ã·»¨9", "Ã·»¨10", "Ã·»¨J", "Ã·»¨Q", "Ã·»¨K", //Ã·»¨
	"ºìÌÒ1", "ºìÌÒ2", "ºìÌÒ3", "ºìÌÒ4", "ºìÌÒ5", "ºìÌÒ6", "ºìÌÒ7", "ºìÌÒ8", "ºìÌÒ9", "ºìÌÒ10", "ºìÌÒJ", "ºìÌÒQ", "ºìÌÒK", //ºìÌÒ
	"ºÚÌÒ1", "ºÚÌÒ2", "ºÚÌÒ3", "ºÚÌÒ4", "ºÚÌÒ5", "ºÚÌÒ6", "ºÚÌÒ7", "ºÚÌÒ8", "ºÚÌÒ9", "ºÚÌÒ10", "ºÚÌÒJ", "ºÚÌÒQ", "ºÚÌÒK", //ºÚÌÒ
};

void NNGameLogic::initCard(byte cbCardData[], byte cbMaxCount)
{
	for (int index = 0; index < cbMaxCount && index < MAX_CARD_COUNT; ++index) {
		cbCardData[index] = GAME_CARDS[index];
	}
}

void NNGameLogic::RandCardData(byte cbCardData[], byte cbMaxCount)
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

TTLNN::NNCardType_Result NNGameLogic::checkNNType(byte cbCardData[], dword gameRules)
{
	for (int index = 0; index < MAX_HAND_CARD; ++index) {
		ASSERT(isCardValid(cbCardData[index]));
	}

	TTLNN::NNCardType_Result result;

	// ÎåÐ¡Å£
	if (hasRule(gameRules, TTLNN::NNCardType_WXN)) {
		bool isWXN = true;
		int sum = 0;
		byte maxCard = 0;

		for (int index = 0; index < MAX_HAND_CARD; ++index) {
			if ((cbCardData[index] & MASK_VALUE) > 5) {
				isWXN = false;
				break;
			}

			sum += (cbCardData[index] & MASK_VALUE);

			if (maxCard == 0) {
				maxCard = cbCardData[index];
			} else {
				if ((cbCardData[index] & MASK_VALUE) > (maxCard & MASK_VALUE)) {
					maxCard = cbCardData[index];
				}

				if ((cbCardData[index] & MASK_VALUE) == (maxCard & MASK_VALUE)) {
					maxCard = std::max(maxCard, cbCardData[index]);
				}
			}
		}

        if(isWXN && sum <= 10) {
            result.type = TTLNN::NNCardType_WXN;
            result.centerCard = maxCard;
			for (int index = 0; index < MAX_HAND_CARD; ++index){
				result.isCardSelected[index] = true;
			}
            return result;
        }
    }

	// Õ¨µ¯Å£
	if (hasRule(gameRules, TTLNN::NNCardType_ZDN)) {
		byte centerCard = 0;

		for (int index = 0; index < MAX_HAND_CARD; ++index) {
			bool same = true;
			centerCard = cbCardData[(index + MAX_HAND_CARD + 1) % MAX_HAND_CARD] & MASK_VALUE;

			for (int cardIndex = 2; cardIndex < MAX_HAND_CARD; ++cardIndex) {
				byte valueTemp = cbCardData[(index + MAX_HAND_CARD + cardIndex) % MAX_HAND_CARD] & MASK_VALUE;

				if (valueTemp != centerCard) {
					same = false;
					break;
				}
			}

            if(same) {
				result.type = TTLNN::NNCardType_ZDN;
                result.centerCard = centerCard;
				zeromemory(&result.isCardSelected, sizeof(result.isCardSelected));
				for (int index = 0; index < MAX_HAND_CARD; ++index){
					if ((cbCardData[index] & MASK_VALUE) == (centerCard & MASK_VALUE)) {
						result.isCardSelected[index] = true;
					}
				}
                return result;
            }
        }
    }

	// Îå»¨Å£
	if (hasRule(gameRules, TTLNN::NNGameRule_SpecialRule_WHN)) {
		bool isWHN = true;
		byte maxCard = 0;

		for (int index = 0; index < MAX_HAND_CARD; ++index) {
			if ((cbCardData[index] & MASK_VALUE) <= 0x0a) {
				isWHN = false;
				break;
			}

			if (maxCard == 0) {
				maxCard = cbCardData[index];
			} else {
				if ((cbCardData[index] & MASK_VALUE) >(maxCard & MASK_VALUE)) {
					maxCard = cbCardData[index];
				}

				if ((cbCardData[index] & MASK_VALUE) == (maxCard & MASK_VALUE)) {
					maxCard = std::max(maxCard, cbCardData[index]);
				}
			}
		}

        if(isWHN) {
			result.type = TTLNN::NNCardType_WHN;
            result.centerCard = maxCard;
			for (int index = 0; index < MAX_HAND_CARD; ++index){
				result.isCardSelected[index] = true;
			}
            return result;
        }
    }

	bool hasN = false;

	zeromemory(&result.isCardSelected, sizeof(result.isCardSelected));
    for(int loop_a = 0; loop_a < MAX_HAND_CARD - 2; ++loop_a) {
        byte value_a = cbCardData[loop_a] & MASK_VALUE;
        value_a = value_a > 0x0a ? 0x0a : value_a;

		for (int loop_b = loop_a + 1; loop_b < MAX_HAND_CARD - 1; ++loop_b) {
			byte value_b = cbCardData[loop_b] & MASK_VALUE;
			value_b = value_b > 0x0a ? 0x0a : value_b;

			for (int loop_c = loop_b + 1; loop_c < MAX_HAND_CARD; ++loop_c) {
				byte value_c = cbCardData[loop_c] & MASK_VALUE;
				value_c = value_c > 0x0a ? 0x0a : value_c;

                if((value_a + value_b + value_c) % 0x0a == 0) {
                    hasN = true;
					result.isCardSelected[loop_a] = true;
					result.isCardSelected[loop_b] = true; 
					result.isCardSelected[loop_c] = true;
                    break;
                }
            }

			if (hasN) {
				break;
			}
		}

		if (hasN) {
			break;
		}
	}

	int sum = 0;
	byte maxCard = 0;

	for (int index = 0; index < MAX_HAND_CARD; ++index) {
		if (maxCard == 0) {
			maxCard = cbCardData[index];
		} else {
			if ((cbCardData[index] & MASK_VALUE) >(maxCard & MASK_VALUE)) {
				maxCard = cbCardData[index];
			}

			if ((cbCardData[index] & MASK_VALUE) == (maxCard & MASK_VALUE)) {
				maxCard = std::max(maxCard, cbCardData[index]);
			}
		}

		if (hasN) {
			byte valueTemp = cbCardData[index] & MASK_VALUE;
			valueTemp = valueTemp > 0x0a ? 0x0a : valueTemp;
			sum += valueTemp;
		}
	}

	if (hasN) {
		int nnNum = sum % 10;
		result.type = TTLNN::NNCardType(TTLNN::NNCardType_None + (nnNum == 0 ? 10 : nnNum));
	} else {
		result.type = TTLNN::NNCardType_None;
	}

	result.centerCard = maxCard;

	return result;
}

bool NNGameLogic::isCardValid(byte cardData)
{
	if ((cardData & MASK_COLOR) > 0x30) {
		return false;
	}

	if ((cardData & MASK_VALUE) == 0 || (cardData & MASK_VALUE) > 0x0d) {
		return false;
	}

	return true;
}

bool NNGameLogic::hasRule(dword gameRule, byte rule)
{
	return FvMask::HasAny(gameRule, _MASK_(rule));
}

int NNGameLogic::getCardIndex(byte cardValue)
{
	return (cardValue / 0x10) * 13 + (cardValue & 0xf);
}
