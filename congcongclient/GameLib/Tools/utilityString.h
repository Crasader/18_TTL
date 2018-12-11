#ifndef __UTILITY_STRING_H__
#define __UTILITY_STRING_H__

#include "STD.h"
#include "Game/Script/ScriptData.h"

using namespace script;

namespace utility
{
	//¸Éµô»»ÐÐ·û
	inline void trim(std::string& _str, bool _left = true, bool _right = true)
	{
		if (_right) _str.erase(_str.find_last_not_of(" \t\r") + 1);
		if (_left) _str.erase(0, _str.find_first_not_of(" \t\r"));
	}

	//utf8×ª»»
	inline std::vector<std::string> parseUTF8(const std::string &str)
	{
		int l = str.length();
		std::vector<std::string> ret;
		ret.clear();
		for (int p = 0; p < l; ) {
			int size = 1;
			unsigned char ch = str[p];
			if (ch >= 0x00 && ch <= 0x7F)
			{
				size = 1;
			}
			else if (ch >= 0xC0 && ch <= 0xDF)
			{
				size = 2;
			}
			else if (ch >= 0xE0 && ch <= 0xEF)
			{
				size = 3;
			}
			else if (ch >= 0xF0 && ch <= 0xF7)
			{
				size = 4;
			}
			else if (ch >= 0xF8 && ch <= 0xFB)
			{
				size = 5;
			}
			else if (ch >= 0xFC && ch <= 0xFD)
			{
				size = 6;
			}
			else
			{
				size = 1;
			}
			std::string temp = "";
			temp = str.substr(p, size);
			ret.push_back(temp);
			p += size;
		}
		return ret;
	}

	//×Ó×Ö·û´®×ª»»³Éutf8
	inline std::string subUTF8(const std::string &str, int from, int to)
	{
		if (from > to) return "";
		std::vector<std::string> test = parseUTF8(str);
		if ((int)test.size() < to) return str;
		std::vector<std::string>::iterator iter = test.begin();
		std::string res;
		std::string result;
		for (iter = (test.begin() + from); iter != (test.begin() + to); iter++)
		{
			res += *iter;
		}
		return res;
	}

	//×Ö·û´®²åÈëstr, ²¢»»ÐÐ
	inline std::string lineFeeds(const std::string &str, int iCurrentIndex)
	{
		if (typeid(str) == typeid(std::string) && str.length() > 0 && iCurrentIndex > 0) {
			std::stringstream result;
			std::vector<std::string> temp;
			temp = parseUTF8(str);
			int iStringLength = temp.size();
			for (int i = 0; i < iStringLength; i++) {
				result << temp[i];
				if (iCurrentIndex == i)
					result << "\n";
			}
			return result.str();
		}
		else {
			return "";
		}
	}

	inline bool isNumAZ(const std::string& str)
	{
		int l = str.length();
		for (int p = 0; p < l; p++)
		{
			if ((str[p] >= '0' && str[p] <= '9') ||
				(str[p] >= 'a' && str[p] <= 'z') ||
				(str[p] >= 'A' && str[p] <= 'Z'))
			{
				continue;
			}
			return false;
		}
		return true;
	}

	template<typename T>
	inline T InRang (const T& iValue, const T& iMax)
	{
		T kTemp = iValue;
		if (kTemp < 0)
		{
			kTemp = 0;
		}
		if (kTemp > iMax)
		{
			kTemp = iMax;
		}
		return kTemp;
	}

	template<typename T>
	inline bool IsInRang (const T& iValue, const T& iValue1, const T& iValue2)
	{
		if (iValue1 < iValue2)
		{
			if (iValue >= iValue1 && iValue <= iValue2)
			{
				return true;
			}
			return false;
		}
		if (iValue >= iValue2 && iValue <= iValue1)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline T InRang (const T& iValue, const T& iMin, const T& iMax)
	{
		T kTemp = iValue;
		if (kTemp < iMin)
		{
			kTemp = iMin;
		}
		if (kTemp > iMax)
		{
			kTemp = iMax;
		}
		return kTemp;
	}

	inline void StringToChar(std::string kStringValue, char* pChar, size_t tSize)
	{
		ASSERT(pChar);
		ASSERT(kStringValue.size() < tSize);
		if(pChar && kStringValue.size() < tSize)
			strcpy(pChar, kStringValue.c_str());
	}

	inline void createAttributeDict(std::map<std::string, std::string>& attributeDict, const char **atts)
	{
		if (atts && atts[0])
		{
			for (int i = 0; atts[i]; i += 2)
			{
				std::string key = (char*)atts[i];
				std::string value = (char*)atts[i + 1];
				attributeDict.insert(std::pair<std::string, std::string>(key, value));
			}
		}
	}

	// §å§ä§Ú§Ý§Ú§ä§í §Õ§Ý§ñ §á§Ñ§â§ã§Ú§ß§Ô§Ñ
	template<typename T>
	inline T parseValue(const std::string& _value)
	{
		std::istringstream stream(_value);
		T result;
		stream >> result;
		if (stream.fail())
			return T();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T();
				item = stream.get();
			}
		}
		return result;
	}

	// §à§ä§Õ§Ö§Ý§î§ß§Ñ§ñ §Ú§Þ§á§Ý§Ö§Þ§Ö§ß§ä§Ñ§è§Ú§ñ §á§à§Õ bool
	template<>
	inline bool parseValue(const std::string& _value)
	{
		if (_value == "True" || _value == "true" || _value == "1")
			return true;
		return false;
	}

	// §à§ä§Õ§Ö§Ý§î§ß§Ñ§ñ §Ú§Þ§á§Ý§Ö§Þ§Ö§ß§ä§Ñ§è§Ú§ñ §á§à§Õ char
	template<>
	inline char parseValue(const std::string& _value)
	{
		return (char)parseValue<short>(_value);
	}

	// §à§ä§Õ§Ö§Ý§î§ß§Ñ§ñ §Ú§Þ§á§Ý§Ö§Þ§Ö§ß§ä§Ñ§è§Ú§ñ §á§à§Õ unsigned char
	template<>
	inline unsigned char parseValue(const std::string& _value)
	{
		return (unsigned char)parseValue<unsigned short>(_value);
	}

	inline short parseShort(const std::string& _value)
	{
		return parseValue<short>(_value);
	}

	inline unsigned short parseUShort(const std::string& _value)
	{
		return parseValue<unsigned short>(_value);
	}

	inline int parseInt(const std::string& _value)
	{
		return parseValue<int>(_value);
	}

	inline unsigned int parseUInt(const std::string& _value)
	{
		return parseValue<unsigned int>(_value);
	}

	inline size_t parseSizeT(const std::string& _value)
	{
		return parseValue<size_t>(_value);
	}

	inline float parseFloat(const std::string& _value)
	{
		return parseValue<float>(_value);
	}

	inline double parseDouble(const std::string& _value)
	{
		return parseValue<double>(_value);
	}

	inline bool parseBool(const std::string& _value)
	{
		return parseValue<bool>(_value);
	}

	inline char parseChar(const std::string& _value)
	{
		return parseValue<char>(_value);
	}

	inline unsigned char parseUChar(const std::string& _value)
	{
		return parseValue<unsigned char>(_value);
	}

	// §Õ§Ý§ñ §á§Ñ§â§ã§Ú§ß§Ô§Ñ §ã§Ý§à§Ø§ß§í§ç §ä§Ú§á§à§Ó, §ã§à§ã§ä§à§ñ§ë§Ú§ç §Ú§Ù §á§â§à§ã§ä§í§ç
	template<typename T1, typename T2>
	inline T1 parseValueEx2(const std::string& _value)
	{
		T2 p1, p2;
		std::istringstream stream(_value);
		stream >> p1 >> p2;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2);
	}

	template<typename T1, typename T2>
	inline T1 parseValueEx3(const std::string& _value)
	{
		T2 p1, p2, p3;
		std::istringstream stream(_value);
		stream >> p1 >> p2 >> p3;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2, p3);
	}

	template<typename T1, typename T2>
	inline T1 parseValueEx4(const std::string& _value)
	{
		T2 p1, p2, p3, p4;
		std::istringstream stream(_value);
		stream >> p1 >> p2 >> p3 >> p4;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2, p3, p4);
	}

	inline std::string  valueForKey(const char *key, std::map<std::string, std::string>& dict)
	{
		std::map<std::string, std::string>::iterator it = dict.find(key);
		return it != dict.end() ? it->second.c_str() : "";
	}

	inline int  valueForInt(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseInt(valueForKey(key, dict));
	}

	inline float  valueForFloat(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseFloat(valueForKey(key, dict));
	}

	inline bool  valueForBool(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseBool(valueForKey(key, dict));
	}

	template<typename T>
	inline void split(std::vector<std::string>& _ret, const std::string& _source, const std::string& _delims)
	{
		if (_source == "")
		{
			return;
		}
		size_t start = 0;
		size_t end = _source.find(_delims);
		while (start != _source.npos)
		{
			if (end != _source.npos)
				_ret.push_back(_source.substr(start, end - start));
			else
			{
				_ret.push_back(_source.substr(start));
				break;
			}
			start = end;
			if (start != _source.npos)
			{
				start += _delims.size();
			}
			end = _source.find(_delims, start);
		}
	}

	inline std::vector<std::string> split(const std::string& _source, const std::string& _delims = "\t\n ")
	{
		std::vector<std::string> result;
		utility::split<void>(result, _source, _delims);
		return result;
	}

	template<typename T1, typename T2, typename T3, typename T4>
	inline bool parseComplex(const std::string& _value, T1& _p1, T2& _p2, T3& _p3, T4& _p4)
	{
		std::istringstream stream(_value);

		stream >> _p1 >> _p2 >> _p3 >> _p4;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<typename T1, typename T2, typename T3>
	inline bool parseComplex(const std::string& _value, T1& _p1, T2& _p2, T3& _p3)
	{
		std::istringstream stream(_value);

		stream >> _p1 >> _p2 >> _p3;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<typename T1, typename T2>
	inline bool parseComplex(const std::string& _value, T1& _p1, T2& _p2)
	{
		std::istringstream stream(_value);

		stream >> _p1 >> _p2;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<typename T1>
	inline bool parseComplex(const std::string& _value, T1& _p1)
	{
		std::istringstream stream(_value);

		stream >> _p1;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<>
	inline bool parseComplex<bool>(const std::string& _value, bool& _p1)
	{
		std::string value(_value);
		trim(value);
		if ((value == "True") || (value == "true") || (value == "1"))
		{
			_p1 = true;
			return true;
		}
		else if ((value == "False") || (value == "false") || (value == "0"))
		{
			_p1 = false;
			return true;
		}

		return false;
	}

	inline bool startWith(const std::string& _source, const std::string& _value)
	{
		size_t count = _value.size();
		if (_source.size() < count)
			return false;
		for (size_t index = 0; index < count; ++index)
		{
			if (_source[index] != _value[index])
				return false;
		}
		return true;
	}

	inline bool endWith(const std::string& _source, const std::string& _value)
	{
		size_t count = _value.size();
		if (_source.size() < count)
			return false;
		size_t offset = _source.size() - count;
		for (size_t index = 0; index < count; ++index)
		{
			if (_source[index + offset] != _value[index])
				return false;
		}
		return true;
	}

	typedef std::vector<std::string> VectorString;
	typedef std::map<std::string, VectorString> MapVectorString;

	inline void PushMapVectorString(MapVectorString& _str, const std::string& _key, const std::string& _value)
	{
		MapVectorString::iterator itor = _str.find(_key);
		if (itor != _str.end())
		{
			itor->second.push_back(_value);
		}
		else
		{
			VectorString kString;
			kString.push_back(_value);
			_str.insert(std::map<std::string, VectorString>::value_type(_key, kString));
		}
	}
	//////////////////////////////////////////////////////////////////////////
	inline std::string ostringcat(std::ostringstream& stream)
	{
		return std::string("");
	}

	template<typename T, typename... T2>
	inline std::string ostringcat(std::ostringstream& stream, T t1, T2... arg)
	{
		stream << t1;
		if (sizeof...(arg) > 0) {
			ostringcat(stream, arg...);
		}
		return stream.str();
	}

	inline std::string toString(bool _value)
	{
		return _value ? "true" : "false";
	}

	template<typename T, typename... T2>
	inline std::string toString(T t1, T2... arg)
	{
		std::ostringstream stream;
		return ostringcat(stream, t1, arg...);
	}

	// ¹¦ÄÜº¯Êý
	inline const char* toString(int value)
	{
		static std::string str;
		str.resize(128, '\0');
		sprintf(&str[0], "%d", value);
		return str.c_str();
	}

#ifdef _WIN32
#define LLSTRING "%I64d"
#else
#define LLSTRING "%lld"
#endif

	inline const char* toString(long long value)
	{
		static std::string str;
		str.resize(128, '\0');
		sprintf(&str[0], LLSTRING, value);
		return str.c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	//×Ö·û´®Ìæ»»

	inline void StringReplace(std::string &strBase, std::string strSrc, std::string strDes, bool bReplaceAll = false) {
		std::string::size_type pos = 0;
		std::string::size_type srcLen = strSrc.size();
		std::string::size_type desLen = strDes.size();
		pos = strBase.find(strSrc, pos);
		while (pos != std::string::npos) {
			strBase.replace(pos, srcLen, strDes);
			pos = strBase.find(strSrc, (pos + desLen));
			if (!bReplaceAll) {
				break;
			}
		}
	}

	inline std::string sprintfStringAsKey(std::string& base, const std::string& key) {
		return base;
	}

	template<typename T1, typename... T2>
	inline std::string sprintfStringAsKey(std::string& base, const std::string& key, T1 val, T2... arg) {
		std::string str_val = toString(val);
		utility::StringReplace(base, key, str_val, false);
		if (sizeof...(arg) > 0) {
			sprintfStringAsKey(base, key, arg...);
		}
		return base;
	}

	template<typename T1, typename...T2>
	inline std::string getScriptReplaceValue(std::string kSrc, T1 val, T2...arg) {
		std::string strBase = ScriptData<std::string>(kSrc).Value();
		std::string key = ScriptData<std::string>("StringKey").Value();
		return sprintfStringAsKey(strBase, key, val, arg...);
	}

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////



	inline std::string paseFloat(float fValue, int iCout)
	{
		char kTxt[12];
		std::string kTemp = utility::toString("%.", iCout, "f");
		sprintf(kTxt, kTemp.c_str(), fValue);
		return kTxt;
	}

	inline std::string paseInt(int iValue, int iCout)
	{
		char kTxt[12];
		std::string kTemp = utility::toString("%0", iCout, "d");
		sprintf(kTxt, kTemp.c_str(), iValue);
		return kTxt;
	}

	inline void saveFilePath(std::string kStr)
	{
		kStr += "|";
		static std::vector<std::string> kStrV;
		for (int i = 0; i < (int)kStrV.size(); i++)
		{
			if (kStrV[i] == kStr)
			{
				return;
			}
		}
		kStrV.push_back(kStr);
		std::string kPathTxt = "FilePath";
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


} 

#endif // 
