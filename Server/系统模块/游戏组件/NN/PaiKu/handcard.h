#ifndef _HANDCARD_H_
#define _HANDCARD_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

typedef unsigned char BYTE;

struct handcard
{
	handcard()
	{
		memset(card, 0, sizeof(card));
	}

	handcard(const BYTE value)
	{
		memset(card, value, sizeof(card) - sizeof(BYTE));
		card[4] = 0;
	}

	handcard(const handcard& val)
	{
		*this = val;
	}

	handcard(const BYTE  value[])
	{
		memcpy(card, value, sizeof(card));
	}

	handcard& operator = (const BYTE value)
	{
		memset(card, value, sizeof(card) - sizeof(BYTE));
		card[4] = 0;
		return *this;
	}

	handcard& operator = (const BYTE * value)
	{
		memcpy(card, value, sizeof(card));
		return *this;
	}

	BYTE& operator [](int idx) {
		return card[idx];
	}

	void print() {
		printf("{%d, %d, %d, %d, %d}",
			static_cast<int>(card[0]),
			static_cast<int>(card[1]),
			static_cast<int>(card[2]),
			static_cast<int>(card[3]),
			static_cast<int>(card[4]));
	}

	BYTE card[5];
};

#endif