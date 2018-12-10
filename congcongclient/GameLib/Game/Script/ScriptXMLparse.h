#ifndef __XMLParse_H__
#define __XMLParse_H__

#include <cocos2d.h>
#include "platform/CCSAXParser.h"

namespace script
{
class ScriptXMLparse : public cocos2d::SAXDelegator
{
public:
	ScriptXMLparse(std::string xmlFileName);
	~ScriptXMLparse() {}
public:
	virtual void startElement(void *ctx, const char *name, const char **atts);
	virtual void endElement(void *ctx, const char *name);
	virtual void textHandler(void *ctx, const char *s, int len);
};
}

#endif // __XMLParse_H__