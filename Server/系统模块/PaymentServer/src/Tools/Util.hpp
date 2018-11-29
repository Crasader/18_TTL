#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include "stdafx.h"

namespace tool {

    static void splitString(const std::string& str, const std::string& delimiters, std::vector<std::string>& tokens)
    {
        // Skip delimiters at beginning. 
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        // Find first "non-delimiter". 
        std::string::size_type pos = str.find_first_of(delimiters, lastPos);

        while (std::string::npos != pos || std::string::npos != lastPos) {
            // Found a token, add it to the vector. 
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            // Skip delimiters.  Note the "not_of" 
            lastPos = str.find_first_not_of(delimiters, pos);
            // Find next "non-delimiter" 
            pos = str.find_first_of(delimiters, lastPos);
        }

    }

    //用于微信dingdan生成随机字符串
    static std::string randString(size_t len) 
    {

        std::string tmp;
        if(len == 0)
            return "";
        srand((unsigned)time(NULL));
        //tmp.resize(len);
        for (size_t idx = 0; idx < len; idx++) {
            char rc = (rand() % ('z' - 'A') + 'A');
            if(rc > 'Z' && rc < 'a') rc = '9';
            tmp += rc;
        }
        //tmp[tmp.length() - 1] = 0;
        return tmp;

    }

    static bool isUTF8String(const char * string)
    {
        if (!string)
            return false;

        const unsigned char * bytes = (const unsigned char *)string;
        while (*bytes)
        {
            if ((// ASCII
                 // use bytes[0] <= 0x7F to allow ASCII control characters
                bytes[0] == 0x09 ||
                bytes[0] == 0x0A ||
                bytes[0] == 0x0D ||
                (0x20 <= bytes[0] && bytes[0] <= 0x7E)
                )
                ) {
                bytes += 1;
                continue;
            }

            if ((// non-overlong 2-byte
                (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF)
                )
                ) {
                bytes += 2;
                continue;
            }

            if ((// excluding overlongs
                bytes[0] == 0xE0 &&
                (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                ) ||
                (// straight 3-byte
                ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                    bytes[0] == 0xEE ||
                    bytes[0] == 0xEF) &&
                    (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                    ) ||
                    (// excluding surrogates
                        bytes[0] == 0xED &&
                        (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                        (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                        )
                ) {
                bytes += 3;
                continue;
            }

            if ((// planes 1-3
                bytes[0] == 0xF0 &&
                (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                ) ||
                (// planes 4-15
                (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                    (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                    (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                    (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                    ) ||
                    (// plane 16
                        bytes[0] == 0xF4 &&
                        (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                        (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                        (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                        )
                ) {
                bytes += 4;
                continue;
            }

            return false;
        }

        return true;
    }

}

#endif