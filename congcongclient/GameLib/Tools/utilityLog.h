#pragma once

#include <cocos2d.h>
#include <stdarg.h>

namespace utility
{

extern std::string s_kDebugFileName;

inline void setDebugFileName(std::string kName)
{
	s_kDebugFileName = cocos2d::FileUtils::getInstance()->getWritablePath() + kName + ".txt";
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

	CCLOG("%s", buf);
	std::string kStr = getTimeStr() + ": " + buf;

	std::string kPathTxt = getDebugFileName();
	if (kPathTxt != "")
	{
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