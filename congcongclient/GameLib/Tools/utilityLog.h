#pragma once

#include <cocos2d.h>
#include <stdarg.h>

namespace utility
{

static std::string s_kDebugFileName = "";

inline void setDebugFileName(std::string kName)
{
	s_kDebugFileName = kName + ".txt";
}

inline std::string getDebugFileName()
{
	return s_kDebugFileName;
}

inline std::string getTimeStr()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&t));
	return tmp;
}

inline void filelog(const char * format, ...)
{
	va_list args;
	va_start(args, format);

	const int iMaxLenght = 1024;
	char buf[iMaxLenght];
	vsnprintf(buf, iMaxLenght - 3, format, args);
	strcat(buf, "\n");
	strcat(buf, "\0");

	std::string kStr = getTimeStr() + ": ";// utility::toString(s_kTime, " ", buf);
	CCLOG("%s", buf);

	if (s_kDebugFileName != "")
	{
		std::string kPathTxt = getDebugFileName();
		FILE* file = fopen(kPathTxt.c_str(), "rb+");
		if (!file)
		{
			file = fopen(kPathTxt.c_str(), "wb+");
		}
		if (file) {
			fseek(file, 0L, SEEK_END);
			fwrite(kStr.c_str(), sizeof(char), strlen(kStr.c_str()), file);
			fclose(file);
		}
	}
	va_end(args);
}


}