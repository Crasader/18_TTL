#include "Tools/utilityString.h"
#include "Tools/utilityLog.h"
#include "Tools/utilityMath.h"
#include "Tools/utilityWidget.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma comment(lib,"libiconv.lib")
#endif

namespace utility {
	std::string s_kDebugFileName = "";
}