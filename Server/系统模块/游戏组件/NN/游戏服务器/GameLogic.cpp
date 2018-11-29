#include "StdAfx.h"
#include "FvMask.h"
#include "GameLogic.h"
#include "math.h"

const BYTE CGameLogic::GAME_CARDS[MAX_CARD_COUNT] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, //∑ΩøÈ
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, //√∑ª®
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, //∫ÏÃ“
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, //∫⁄Ã“
};

const std::string CGameLogic::GAME_CARDS_NAME[MAX_CARD_COUNT] = {
    "∑ΩøÈ1", "∑ΩøÈ2", "∑ΩøÈ3", "∑ΩøÈ4", "∑ΩøÈ5", "∑ΩøÈ6", "∑ΩøÈ7", "∑ΩøÈ8", "∑ΩøÈ9", "∑ΩøÈ10", "∑ΩøÈJ", "∑ΩøÈQ", "∑ΩøÈK", //∑ΩøÈ
    "√∑ª®1", "√∑ª®2", "√∑ª®3", "√∑ª®4", "√∑ª®5", "√∑ª®6", "√∑ª®7", "√∑ª®8", "√∑ª®9", "√∑ª®10", "√∑ª®J", "√∑ª®Q", "√∑ª®K", //√∑ª®
    "∫ÏÃ“1", "∫ÏÃ“2", "∫ÏÃ“3", "∫ÏÃ“4", "∫ÏÃ“5", "∫ÏÃ“6", "∫ÏÃ“7", "∫ÏÃ“8", "∫ÏÃ“9", "∫ÏÃ“10", "∫ÏÃ“J", "∫ÏÃ“Q", "∫ÏÃ“K", //∫ÏÃ“
    "∫⁄Ã“1", "∫⁄Ã“2", "∫⁄Ã“3", "∫⁄Ã“4", "∫⁄Ã“5", "∫⁄Ã“6", "∫⁄Ã“7", "∫⁄Ã“8", "∫⁄Ã“9", "∫⁄Ã“10", "∫⁄Ã“J", "∫⁄Ã“Q", "∫⁄Ã“K", //∫⁄Ã“
};

CGameLogic::CGameLogic() {

}

CGameLogic::~CGameLogic() {

}

void CGameLogic::initCard(BYTE cbCardData[], BYTE cbMaxCount) {
    for (int index = 0; index < cbMaxCount && index < MAX_CARD_COUNT; ++index) {
        cbCardData[index] = GAME_CARDS[index];
    }
}

void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount) {
    std::vector<BYTE> cardTemp;

    for (int index = 0; index < cbMaxCount; ++index) {
        cardTemp.push_back(cbCardData[index]);
    }

    for (int index = 0; index < cbMaxCount; ++index) {
        int random = rand() % cardTemp.size();
        cbCardData[index] = cardTemp.at(random);
        cardTemp.erase(find(cardTemp.begin(), cardTemp.end(), cardTemp.at(random)));
    }
}

NNCardType_Result CGameLogic::checkNNType(BYTE cbCardData[], DWORD gameRules) {
    for (int index = 0; index < MAX_HAND_CARD; ++index) {
        ASSERT(isCardValid(cbCardData[index]));
    }

    NNCardType_Result result;

    // ŒÂ–°≈£
    if (hasRule(gameRules, NNGameRule_SpecialRule_WXN)) {
        bool isWXN = true;
        int sum = 0;
        BYTE maxCard = 0;

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
                    maxCard = max(maxCard, cbCardData[index]);
                }
            }
        }

        if (isWXN && sum <= 10) {
            result.type = NNCardType_WXN;
            result.centerCard = maxCard;
            for (int index = 0; index < MAX_HAND_CARD; ++index) {
                result.isCardSelected[index] = true;
            }
            return result;
        }
    }

    // ’®µØ≈£
    if (hasRule(gameRules, NNGameRule_SpecialRule_ZDN)) {
        BYTE centerCard = 0;

        for (int index = 0; index < MAX_HAND_CARD; ++index) {
            bool same = true;
            centerCard = cbCardData[(index + MAX_HAND_CARD + 1) % MAX_HAND_CARD] & MASK_VALUE;

            for (int cardIndex = 2; cardIndex < MAX_HAND_CARD; ++cardIndex) {
                BYTE valueTemp = cbCardData[(index + MAX_HAND_CARD + cardIndex) % MAX_HAND_CARD] & MASK_VALUE;

                if (valueTemp != centerCard) {
                    same = false;
                    break;
                }
            }

            if (same) {
                result.type = NNCardType_ZDN;
                result.centerCard = centerCard;
                ZeroMemory(&result.isCardSelected, sizeof(result.isCardSelected));
                for (int index = 0; index < MAX_HAND_CARD; ++index) {
                    if ((cbCardData[index] & MASK_VALUE) == (centerCard & MASK_VALUE)) {
                        result.isCardSelected[index] = true;
                    }
                }
                return result;
            }
        }
    }

    // ∫˘¬´≈£
    if (hasRule(gameRules, NNGameRule_SpecialRule_HLN)) {
        BYTE centerCard = 0;

        byte cardValues[MAX_HAND_CARD] = {0};
        byte cardCounts[MAX_HAND_CARD] = { 0 };
        for (int index = 0; index < MAX_HAND_CARD; ++index) {
            for (int valueIndex = 0; valueIndex < MAX_HAND_CARD; ++valueIndex) {
                if (0 == cardValues[valueIndex]) {
                    cardValues[valueIndex] = cbCardData[index];
                    cardCounts[valueIndex] = 1;
                    break;
                }

                if ((cardValues[valueIndex] & MASK_VALUE) == (cbCardData[index] & MASK_VALUE)) {
                    cardCounts[valueIndex]++;
                    break;
                }
            }
        }

        bool isHLN = (cardCounts[0] + cardCounts[1]) == 5;
        centerCard = cardCounts[0] == 3 ? cardValues[0] : cardValues[1];
        if (isHLN) {
            result.type = NNCardType_HLN;
            result.centerCard = centerCard;
            ZeroMemory(&result.isCardSelected, sizeof(result.isCardSelected));
            for (int index = 0; index < MAX_HAND_CARD; ++index) {
                if ((cbCardData[index] & MASK_VALUE) == (centerCard & MASK_VALUE)) {
                    result.isCardSelected[index] = true;
                }
            }
            return result;
        }
    }

    // Õ¨ª®≈£
    if (hasRule(gameRules, NNGameRule_SpecialRule_THN)) {
        bool isTHN = true;
        BYTE cardColor = 0;
        BYTE centerCard = 0;
        for (int index = 0; index < MAX_HAND_CARD; ++index) {
            if (index == 0) {
                cardColor = cbCardData[index] & MASK_COLOR;
                centerCard = cbCardData[index];
                continue;
            }

            if (cardColor != (cbCardData[index] & MASK_COLOR)) {
                isTHN = false;
                break;
            }

            if ((centerCard & MASK_VALUE) < (cbCardData[index] & MASK_VALUE)) {
                centerCard = cbCardData[index];
            }

            if ((centerCard & MASK_VALUE) == (cbCardData[index] & MASK_VALUE)) {
                centerCard = max(centerCard, cbCardData[index]);
            }
        }


        if (isTHN) {
            result.type = NNCardType_THN;
            result.centerCard = centerCard;
            ZeroMemory(&result.isCardSelected, sizeof(result.isCardSelected));
            for (int index = 0; index < MAX_HAND_CARD; ++index) {
                result.isCardSelected[index] = true;
            }
            return result;
        }
    }

    // ŒÂª®≈£
    if (hasRule(gameRules, NNGameRule_SpecialRule_WHN)) {
        bool isWHN = true;
        BYTE maxCard = 0;

        for (int index = 0; index < MAX_HAND_CARD; ++index) {
            if ((cbCardData[index] & MASK_VALUE) < 0x0a) {
                isWHN = false;
                break;
            }

            if (maxCard == 0) {
                maxCard = cbCardData[index];
            } else {
                if ((cbCardData[index] & MASK_VALUE) > (maxCard & MASK_VALUE)) {
                    maxCard = cbCardData[index];
                }

                if ((cbCardData[index] & MASK_VALUE) == (maxCard & MASK_VALUE)) {
                    maxCard = max(maxCard, cbCardData[index]);
                }
            }
        }

        if (isWHN) {
            result.type = NNCardType_WHN;
            result.centerCard = maxCard;
            for (int index = 0; index < MAX_HAND_CARD; ++index) {
                result.isCardSelected[index] = true;
            }
            return result;
        }
    }

    // À≥◊”≈£
    if (hasRule(gameRules, NNGameRule_SpecialRule_SZN)) {
        BYTE maxCard = 0;

        BYTE cardValues[MAX_HAND_CARD] = {0};
        for (int index = 0; index < MAX_HAND_CARD; ++index) {
            cardValues[index] = cbCardData[index] & MASK_VALUE;

            if (maxCard == 0) {
                maxCard = cbCardData[index];
            } else {
                if ((cbCardData[index] & MASK_VALUE) > (maxCard & MASK_VALUE)) {
                    maxCard = cbCardData[index];
                }

                if ((cbCardData[index] & MASK_VALUE) == (maxCard & MASK_VALUE)) {
                    maxCard = max(maxCard, cbCardData[index]);
                }
            }
        }

        for (int loop_a = 0; loop_a < MAX_HAND_CARD - 1; ++loop_a) {
            for (int loop_b = loop_a + 1; loop_b < MAX_HAND_CARD; ++loop_b) {
                if (cardValues[loop_a] > cardValues[loop_b]) {
                    std::swap(cardValues[loop_a], cardValues[loop_b]);
                }
            }
        }

        bool isSZN = true;
        for (int index = 0; index < MAX_HAND_CARD - 1; ++index) {
            if (cardValues[index] + 1 != cardValues[index + 1]) {
                isSZN = false;
                break;
            }
        }

        if (isSZN) {
            result.type = NNCardType_SZN;
            result.centerCard = maxCard;
            for (int index = 0; index < MAX_HAND_CARD; ++index) {
                result.isCardSelected[index] = true;
            }
            return result;
        }
    }

    bool hasN = false;

    ZeroMemory(&result.isCardSelected, sizeof(result.isCardSelected));
    for (int loop_a = 0; loop_a < MAX_HAND_CARD - 2; ++loop_a) {
        BYTE value_a = cbCardData[loop_a] & MASK_VALUE;
        value_a = value_a > 0x0a ? 0x0a : value_a;

        for (int loop_b = loop_a + 1; loop_b < MAX_HAND_CARD - 1; ++loop_b) {
            BYTE value_b = cbCardData[loop_b] & MASK_VALUE;
            value_b = value_b > 0x0a ? 0x0a : value_b;

            for (int loop_c = loop_b + 1; loop_c < MAX_HAND_CARD; ++loop_c) {
                BYTE value_c = cbCardData[loop_c] & MASK_VALUE;
                value_c = value_c > 0x0a ? 0x0a : value_c;

                if ((value_a + value_b + value_c) % 0x0a == 0) {
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
    int maxCard = 0;

    for (int index = 0; index < MAX_HAND_CARD; ++index) {
        if (maxCard == 0) {
            maxCard = cbCardData[index];
        } else {
            if ((cbCardData[index] & MASK_VALUE) > (maxCard & MASK_VALUE)) {
                maxCard = cbCardData[index];
            }

            if ((cbCardData[index] & MASK_VALUE) == (maxCard & MASK_VALUE)) {
                maxCard = max(maxCard, cbCardData[index]);
            }
        }

        if (hasN) {
            BYTE valueTemp = cbCardData[index] & MASK_VALUE;
            valueTemp = valueTemp > 0x0a ? 0x0a : valueTemp;
            sum += valueTemp;
        }
    }

    if (hasN) {
        int nnNum = sum % 10;
        result.type = NNCardType(NNCardType_None + (nnNum == 0 ? 10 : nnNum));
    } else {
        result.type = NNCardType_None;
    }

    result.centerCard = maxCard;

    return result;
}

BOOL CGameLogic::comparePlayerCards(NNCardType_Result result_1, NNCardType_Result result_2) {
    if (result_1.type > result_2.type) {
        return TRUE;
    }

    if (result_1.type < result_2.type) {
        return FALSE;
    }

    if ((result_1.centerCard & MASK_VALUE) > (result_2.centerCard & MASK_VALUE)) {
        return TRUE;
    }

    if ((result_1.centerCard & MASK_VALUE) < (result_2.centerCard & MASK_VALUE)) {
        return FALSE;
    }

    return result_1.centerCard > result_2.centerCard;
}

int CGameLogic::getNNRatio(NNCardType_Result result, DWORD gameRules) {
    if (hasRule(gameRules, NNGameRule_Ratio_0)) {
        switch (result.type) {
			case NNCardType_NN:
				return CTR1_NN;
			case NNCardType_N9:
				return CTR1_N9;
			case NNCardType_N8:
				return CTR1_N8;
			case NNCardType_N7:
                return CTR1_N7;
			case NNCardType_WXN:
				return CardTypeRatio_WXN;
			case NNCardType_ZDN:
				return CardTypeRatio_ZDN;
			case NNCardType_HLN:
				return CardTypeRatio_HLN;
			case NNCardType_THN:
				return CardTypeRatio_THN;
			case NNCardType_WHN:
				return CardTypeRatio_WHN;
			case NNCardType_SZN:
				return CardTypeRatio_SZN;
            default:
                return 1;
        }
    } else {
        switch (result.type) {
			case NNCardType_NN:
				return CTR2_NN;
			case NNCardType_N9:
				return CTR2_N9;
			case NNCardType_N8:
				return CTR2_N8;
			case NNCardType_N7:
				return CTR2_N7;
			case NNCardType_WXN:
				return CardTypeRatio_WXN;
			case NNCardType_ZDN:
				return CardTypeRatio_ZDN;
			case NNCardType_HLN:
				return CardTypeRatio_HLN;
			case NNCardType_THN:
				return CardTypeRatio_THN;
			case NNCardType_WHN:
				return CardTypeRatio_WHN;
			case NNCardType_SZN:
				return CardTypeRatio_SZN;
			default:
				return 1;
        }
    }
}

bool CGameLogic::isCardValid(BYTE cardData) {
    if ((cardData & MASK_COLOR) > 0x30) {
        return false;
    }

    if ((cardData & MASK_VALUE) == 0 || (cardData & MASK_VALUE) > 0x0d) {
        return false;
    }

    return true;
}

bool CGameLogic::hasRule(DWORD gameRule, BYTE rule) {
    return FvMask::HasAny(gameRule, _MASK_(rule));
}
